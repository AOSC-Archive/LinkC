#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_mainwindow.h"
#include "linkc_def.h"
#include "linkc_error.h"
#include "linkc_package.h"
#include "linkc_package_ctl.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->hide();
    Buffer = new char[STD_PACKAGE_SIZE];
    Package= new char[STD_PACKAGE_SIZE];
    LoginW = new LoginWindow;
    LoginW->show();

    /*初始化套接字*/
    Socket = new TCP_Socket;
    struct sockaddr_in  NetAddr;
    memset((void*)&NetAddr,0,sizeof(struct sockaddr_in));
    NetAddr.sin_family  = AF_INET;
    NetAddr.sin_port    = htons(2341);
    inet_aton("127.0.0.1",(struct in_addr*)&NetAddr.sin_addr.s_addr);
    Socket->SetDestAddr(NetAddr);

    connect(LoginW,SIGNAL(SIGN_LoginStart(LoginData)),this,SLOT(DoLogin(LoginData)));
    connect(LoginW,SIGNAL(SIGN_QuitButtonClicked()),this,SLOT(ExitProgram()));
    connect(this,SIGNAL(LoginStatus(bool)),this,SLOT(setVisible(bool)));
    connect(this,SIGNAL(LoginStatus(bool)),LoginW,SLOT(setHidden(bool)));
}

MainWindow::~MainWindow(){
    delete (char*)Buffer;
    delete (char*)Package;
    delete Socket;
    delete ui;
}

void MainWindow::DoLogin(LoginData Data){
    LinkC_Debug("连接到服务器",LINKC_STARTED);
    if(Socket->Connect()==LINKC_FAILURE){
        QMessageBox::warning(this,tr("失败"),tr("链接到服务器失败"),QMessageBox::Yes);
        LinkC_Debug("连接到服务器",LINKC_FAILURE);
        emit LoginStatus(false);
    }
    LinkC_Debug("连接到服务器",LINKC_DONE);
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
    LinkC_Debug("用户密码检验",LINKC_SUCCESS);

    Socket->Recv(Buffer,STD_PACKAGE_SIZE,0);
    if(_UnPackage(Buffer,STD_PACKAGE_SIZE,Package) == LINKC_FAILURE){
        emit LoginStatus(false);
        return;
    }
    if(ntohs(((MessageHeader*)Package)->StatusCode) == SET_STATUS_FAILURE){
        emit LoginStatus(false);
        return;
    }
    LinkC_Debug("状态设置",LINKC_SUCCESS);

    emit LoginStatus(true);
    return;
}

void MainWindow::ExitProgram(){
    free(LoginW);
    this->close();
    exit(0);
}
