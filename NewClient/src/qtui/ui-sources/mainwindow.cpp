#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_mainwindow.h"
#include "linkc_def.h"
#include "linkc_error.h"
#include "linkc_package.h"
#include "linkc_package_ctl.h"
#include "friendslist.h"
#include "linkc_package.h"
#include "chatdialog.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->hide();
    Buffer = new char[STD_PACKAGE_SIZE];
    Package= new char[STD_PACKAGE_SIZE];
    LoginW = new LoginWindow;
    User   = new UserData;
    FList  = new FriendsList(this);
    FList->setGeometry(10,50,100,200);
    LoginW->show();

    /*初始化套接字*/
    Socket = new TCP_Socket;
    struct sockaddr_in  NetAddr;
    memset((void*)&NetAddr,0,sizeof(struct sockaddr_in));
    NetAddr.sin_family  = AF_INET;
    NetAddr.sin_port    = htons(2341);
    inet_aton("117.59.12.104",(struct in_addr*)&NetAddr.sin_addr.s_addr);
    Socket->SetDestAddr(NetAddr);

    connect(LoginW,SIGNAL(SIGN_LoginStart(LoginData)),this,SLOT(DoLogin(LoginData)));
    connect(LoginW,SIGNAL(SIGN_QuitButtonClicked()),this,SLOT(ExitProgram()));
    connect(this,SIGNAL(LoginStatus(bool)),this,SLOT(setVisible(bool)));
    connect(this,SIGNAL(LoginStatus(bool)),LoginW,SLOT(setHidden(bool)));
    connect(FList,SIGNAL(ChatTo(UserData)),this,SLOT(OpenChatDialog(UserData)));
    connect(ui->RefreshButton,SIGNAL(clicked()),this,SLOT(DoGetFriendsData()));
}

MainWindow::~MainWindow(){
    delete (char*)Buffer;
    delete (char*)Package;
    delete Socket;
    delete ui;
}

void MainWindow::DoLogin(LoginData Data){
    LinkC_Debug(tr("连接到服务器").toUtf8().data(),LINKC_STARTED);
    if(Socket->Connect()==LINKC_FAILURE){
        QMessageBox::warning(this,tr("失败"),tr("链接到服务器失败"),QMessageBox::Yes);
        LinkC_Debug(tr("连接到服务器").toUtf8().data(),LINKC_FAILURE);
        emit LoginStatus(false);
        exit(0);
    }
    LinkC_Debug(tr("连接到服务器").toUtf8().data(),LINKC_DONE);
    ((MessageHeader*)Buffer)->ActionType = USER_LOGIN;
    memcpy((void*)((char*)Buffer+4),(void*)&Data,sizeof(LoginData));
    int Length = _Package(Buffer,sizeof(MessageHeader)+sizeof(LoginData),NORMAL_MESSAGE,Package);
    Socket->Send(Package,Length,0);

    Socket->Recv(Buffer,STD_PACKAGE_SIZE,0);
    if(_UnPackage(Buffer,STD_PACKAGE_SIZE,Package) == LINKC_FAILURE){
        emit LoginStatus(false);
        return;
    }
    if(ntohs(((MessageHeader*)Package)->StatusCode) == LOGIN_FAILURE){
        emit LoginStatus(false);
        return;
    }
    LinkC_Debug(tr("用户密码检验").toUtf8().data(),LINKC_SUCCESS);

    Socket->Recv(Buffer,STD_PACKAGE_SIZE,0);
    if(_UnPackage(Buffer,STD_PACKAGE_SIZE,Package) == LINKC_FAILURE){
        emit LoginStatus(false);
        return;
    }
    if(ntohs(((MessageHeader*)Package)->StatusCode) == SET_STATUS_FAILURE){
        emit LoginStatus(false);
        return;
    }
    LinkC_Debug(tr("状态设置").toUtf8().data(),LINKC_SUCCESS);

    emit LoginStatus(true);
    DoGetSelfData();
    DoGetFriendsData();
    return;
}

int MainWindow::DoGetSelfData(){
    LinkC_Debug(tr("获取自身数据").toUtf8().data(),LINKC_STARTED);
    bzero(Buffer, STD_PACKAGE_SIZE);
    bzero(Package,STD_PACKAGE_SIZE);
    ((MessageHeader*)Buffer)->ActionType = (RQUEST_DATA|SELF_DATA);
    int Length = _Package(Buffer,sizeof(MessageHeader),NORMAL_MESSAGE,Package);
    Socket->Send(Package,Length,0);
    LinkC_Debug(tr("个人数据申请要求发送").toUtf8().data(),LINKC_DONE);
    LinkC_Debug(tr("等待服务器回应").toUtf8().data(),LINKC_STARTED);
    if(Socket->Recv(Buffer,STD_PACKAGE_SIZE,0) == LINKC_FAILURE){
        LinkC_Debug(tr("接收数据").toUtf8().data(),LINKC_FAILURE);
        return LINKC_FAILURE;
    }
    _UnPackage(Buffer,STD_PACKAGE_SIZE,Package);
    if(((MessageHeader*)Package)->ActionType != (RETURN_DATA|SELF_DATA)){
        LinkC_Debug(tr("服务端返回数据错误").toUtf8().data(),LINKC_WARNING);
        return LINKC_FAILURE;
    }
    if(((MessageHeader*)Package)->StatusCode == htons(GET_DATA_FAILURE)){
        LinkC_Debug(tr("服务端获取数据").toUtf8().data(),LINKC_FAILURE);
        return LINKC_FAILURE;
    }
    LinkC_Debug(tr("接收个人数据").toUtf8().data(),LINKC_DONE);
    memcpy((void*)User,((char*)Package)+sizeof(MessageHeader),sizeof(UserData));
    LinkC_Debug(tr("获取自身数据").toUtf8().data(),LINKC_SUCCESS);
    ui->label->setText(tr(User->NickName));
    return LINKC_SUCCESS;
}

int MainWindow::DoGetFriendsData(){
    bzero(Buffer,STD_PACKAGE_SIZE);
    bzero(Package,STD_PACKAGE_SIZE);
    LinkC_Debug(tr("刷新好友列表").toUtf8().data(),LINKC_STARTED);
    ((MessageHeader*)Buffer)->ActionType = (RQUEST_DATA|USER_DATA);
    ((RequestUser*)(char*)Buffer+sizeof(MessageHeader))->UID = htonl(0);
    int Length = _Package(Buffer,sizeof(MessageHeader)+sizeof(RequestUser),NORMAL_MESSAGE,Package);
    Socket->Send(Package,Length,0);
    LinkC_Debug(tr("发送请求").toUtf8().data(),LINKC_DEBUG);
    bzero(Buffer,STD_PACKAGE_SIZE);
    bzero(Package,STD_PACKAGE_SIZE);
    Socket->Recv(Package,STD_PACKAGE_SIZE,0);
    Length = ntohs(((PackageHeader*)Package)->MessageLength)-sizeof(MessageHeader);
    _UnPackage(Package,STD_PACKAGE_SIZE,Buffer);
    if(((MessageHeader*)Buffer)->ActionType != (RETURN_DATA|FRIENDS_DATA)){
        LinkC_Debug(tr("服务端返回数据错误").toUtf8().data(),LINKC_DEBUG);
        LinkC_Debug(tr("获取全部好友资料").toUtf8().data(),LINKC_FAILURE);
        return LINKC_FAILURE;
    }else if(((MessageHeader*)Buffer)->StatusCode != htons(GET_DATA_SUCCESS)){
        LinkC_Debug(tr("服务端获取数据失败").toUtf8().data(),LINKC_DEBUG);
        LinkC_Debug(tr("获取全部好友资料").toUtf8().data(),LINKC_FAILURE);
        return LINKC_FAILURE;
    }else if(((MessageHeader*)Buffer)->StatusCode == htons(NO_DATA)){
        LinkC_Debug(tr("你没有任何好友").toUtf8().data(),LINKC_DEBUG);
        LinkC_Debug(tr("获取全部好友资料").toUtf8().data(),LINKC_SUCCESS);
        return LINKC_SUCCESS;
    }
    LinkC_Debug("刷新好友资料",LINKC_SUCCESS);
    int Count = Length / sizeof(UserData);
    FList->ClearFriendsList();
    FList->SetFriendCount(Count);
    int i;
    for(i=0;i<Count;i++){
        FList->AddFriendToList(((UserData*)((char*)Buffer+sizeof(MessageHeader)+i*sizeof(UserData))));
    }
    return LINKC_SUCCESS;
}


void MainWindow::OpenChatDialog(UserData Data){
    ChatDialog *log;
    if(!ChatDialogMap.contains(Data.UID)){
        log = new ChatDialog(Data);
        log->show();
        ChatDialogMap.insert(Data.UID, log);
    }else{
        ChatDialogiterator = ChatDialogMap.find(Data.UID);
        log = ChatDialogiterator.value();
        log->show();
    }
}

void MainWindow::ExitProgram(){
    free(LoginW);
    this->close();
    exit(0);
}
