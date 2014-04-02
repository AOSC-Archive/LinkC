/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 2, 2014
 */
#include "MainWindow.h"
#include "Csocket.h"
#include "linkc_types.h"
#include "LinkC_Label.h"
#include "LinkC_GUI.h"
#include "linkc_network_protocol.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <QVariant>      // 为了注册数据类型


char buffer[MAXBUF];
int length;
int flag;
LoginWindow *s;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent){
    Top         = new QWidget(this);    // 顶部
    MainWidget  = new QWidget(this);    // 中部
    MainSetupMenu=new SetupMenu(this);  // 底部设置栏
    s           = new LoginWindow;      // 登录界面
    layout      = new QGridLayout;      // 主layout
    TopLayout   = new QGridLayout;      // 顶部layout
    MainLayout  = new QVBoxLayout;      // 中间layout
    area        = new FriendArea;       //
    tab         = new QTabWidget;       //
    package     = new char[MAXBUF];     //
    setWindowTitle("Main_Window");      // 标题
    Connection_state = NetworkInit();   // 初始化网络
    if(Connection_state == -1){
        exit(0);  // 如果链接失败，退出
    }
    Recver      = new TCP_MessageRecver(&server);
    this->setCentralWidget(MainWidget); // 设置主widget
    MainWidget->setLayout(layout);      // 设置主layout
    layout->addLayout(MainLayout,0,0,1,1);
    this->setMaximumSize(300,700);
    this->setMinimumSize(150,300);

//############注册数据类型##########
    LinkC_Sys_Status D1;
    LinkC_Friend_Data D2;
    LinkC_User_Message D3;
    LinkC_User_Request D4;
    LinkC_User_Data D5;
    QVariant DataVar;
    DataVar.setValue(D1);
    qRegisterMetaType<LinkC_Sys_Status>("LinkC_Sys_Status");
    DataVar.setValue(D2);
    qRegisterMetaType<LinkC_Friend_Data>("LinkC_Friend_Data");
    DataVar.setValue(D3);
    qRegisterMetaType<LinkC_User_Message>("LinkC_User_Message");
    DataVar.setValue(D4);
    qRegisterMetaType<LinkC_User_Message>("LinkC_User_Request");
    DataVar.setValue(D5);
    qRegisterMetaType<LinkC_User_Data>("LinkC_User_Data");

//############初始化顶部############
        Top->setGeometry(0,0,this->width(),50); //设置大小
        QHBoxLayout *line = new QHBoxLayout;
        Top->setLayout(line);
        line->addSpacing(50);
        line->addLayout(TopLayout);
        QLabel *Name = new QLabel(tr("Tricks"));
        QLabel *Stat = new QLabel(tr("Online"));
        QLabel *Quit = new QLabel(tr("Quit"));
        QLabel *Exit = new QLabel(tr("Exit"));
        TopLayout->addWidget(Name,0,0,1,1);
        TopLayout->addWidget(Stat,1,0,1,1);
        TopLayout->addWidget(Quit,0,1,1,1);
        TopLayout->addWidget(Exit,1,1,1,1);
        Top->show();
        LinkC_Label *head = new LinkC_Label(this);
        QPixmap pic("01.jpg");
        head->setPixmap(pic);
        head->setGeometry(0,0,50,50);
//############连接####################
        this->connect(area, SIGNAL(ChatTo(LinkC_Friend_Data)),this, SLOT(FriendLabelClicked(LinkC_Friend_Data)));
        this->connect(Recver,SIGNAL(UserMessage(LinkC_User_Message)),this,SLOT(UserMessage(LinkC_User_Message)));
        this->connect(Recver,SIGNAL(SysActionStatus(LinkC_Sys_Status)),this,SLOT(SysActionStatus(LinkC_Sys_Status)));
        this->connect(MainSetupMenu,SIGNAL(SIG_Quit()),this,SLOT(SLOT_Quit()));
        this->connect(MainSetupMenu,SIGNAL(SIG_Refresh_User_Info()),this,SLOT(SLOT_Refresh_User_Info()));
        this->connect(MainSetupMenu,SIGNAL(SIG_Refresh_Friend_List()),this,SLOT(SLOT_Refresh_Friend_List()));
        this->connect(Recver,SIGNAL(SysFriendsList(void*,int)),this,SLOT(SLOT_RecvFriendList(void*,int)));
        head->show();
//############配置编码#################
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForTr(codec);
        QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
        QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
//############初始化中间部分############
    MainLayout->addSpacing(50);
    MainLayout->addWidget(area);
    MainLayout->addSpacing(25);
    MainSetupMenu->setText(tr("菜单"));

//############登录####################
    Login();        //登录
    delete s;
    InitFriendList();
    Recver->start();
}

MainWindow::~MainWindow(){
    length = pack_message(EXIT,NULL,0,buffer);
    server.Send_msg(buffer,length,MSG_DONTWAIT);

    LinkC_Debug("Main_Window\t= [EXITED]");
}

int MainWindow::NetworkInit(void){
	int i;
    server.Set_IP(SERVER_IP);
    server.Debug_Csocket_IP();
	server.Set_Port(2341);
	server.Debug_Csocket_Port();
    i = server.start_connect();
    if (i == -1)
	{
		QMessageBox::warning(0,"Waring","Connection refused!",QMessageBox::Yes);
        return -1;
	}
    server.Debug_Csocket_Sockfd();
    length = pack_message(CONNECTION,NULL,0,buffer);
    server.Send_msg(buffer,length,0);
    bzero(buffer,STD_PACKAGE_SIZE);
    length = server.TCP_Recv(buffer,STD_PACKAGE_SIZE,0);
    flag = get_message_header(buffer);
    if (flag == SYS_ACTION_STATUS){
        unpack_message(buffer,package);
        if(((LSS *)package)->Action == CONNECTION){
            if(((LSS *)package)->Status == LINKC_SUCCESS){
                printf ("Debug >> Connect\t= [Success]\n");
                return 0;
            }
            else if(((LSS *)package)->Status == LINKC_FAILURE){
                printf ("Debug >> Connect\t= [Failure]\n");
                return -1;
            }
        }
    }
    printf("Flag = %d\n",flag);
    return -1;
}

int MainWindow::Login(){

	struct login_data st;
	int i;
	while(1){

		i = s->exec();
		if (i == 1){
			i = s->GetLoginData(st);
			if (i == -1)continue;
            length = pack_message(LOGIN,(void *)&st,LUL_L,package);
            server.Send_msg(package,length,0);
            server.TCP_Recv(buffer,STD_PACKAGE_SIZE,0);
            flag = get_message_header(buffer);
            if (flag == SYS_ACTION_STATUS){
                unpack_message(buffer,package);
                if(((LSS *)package)->Action == LOGIN){
                    if(((LSS *)package)->Status == LINKC_SUCCESS){
                        LinkC_Debug("Login\t\t= [Success]");
                        this->show();
                        return 0;
                    }
                    else if(((LSS *)package)->Status == LINKC_FAILURE){
                        LinkC_Debug("Login\t\t= [Failure]");
                        QMessageBox::warning(0,"Waring","Login Faliure!",QMessageBox::Yes);
                        continue;
                    }
                    else if(((LSS *)package)->Status == LINKC_LIMITED){
                        QMessageBox::warning(0,"Waring","Please Try later!",QMessageBox::Yes);
                        LinkC_Debug("Login\t\t= [Limited]");
                        exit(0);
                    }
                }
            }
            LinkC_Debug("Login\t\t= [MessageHeader Incorrect]");
            exit(0);
		}
        else
            exit(0);
	}
}

void MainWindow::resizeEvent(QResizeEvent *){
    Top->setGeometry(0,0,this->width(),50);
    MainSetupMenu->setGeometry(10,this->height()-33,60,30);
}

void MainWindow::closeEvent(QCloseEvent *){
    for ( ChatDialogiterator = ChatDialogMap.begin(); ChatDialogiterator != ChatDialogMap.end(); ++ChatDialogiterator )
                ChatDialogiterator.value()->~ChatDialog();
}

int MainWindow::InitFriendList(){
    area->clear();
    int flag;
    ((LUR *)package)->Action = USER_FRIEND_DATA;
    ((LUR *)package)->UID = 0;
    length = pack_message(USER_REQUEST,package,LUR_L,buffer);
    server.Send_msg(buffer,length,0);     // Send for Getting Friend Data
    server.TCP_Recv(buffer,STD_PACKAGE_SIZE,0);                // recv state
    printf("Recved!\n");
    flag = get_message_header(buffer);
    if(flag == SYS_ACTION_STATUS){
        unpack_message(buffer,package);
        if(((LSS *)package)->Action == USER_FRIENDS_DATA){
            if(((LSS *)package)->Status == LINKC_SUCCESS)
                LinkC_Debug("State\t\t= [Success]");
            else{
                if(((LSS *)package)->Status == LINKC_FAILURE)
                    LinkC_Debug("State\t\t= [Failure]");
                else
                    LinkC_Debug("Friend\t\t= [NULL]");
                return 0;
            }
        }
    }
    flag = non_std_m_message_recv(server.GetSockfd(),sizeof(LinkC_Friend_Data),package);
    area->setFriendCount(flag);                       // save Friend count
    LinkC_Friend_Data *ffb = new LinkC_Friend_Data[area->FriendCount()];    // new memory
    memcpy(ffb,package,area->FriendCount() * sizeof(LinkC_Friend_Data));  // Save Friend Data

    int i;
    for(i=0;i<area->FriendCount();i++)
        area->AddFriendToLayout(ffb[i]);

    return 0;
}


void MainWindow::ChatWith(LinkC_Friend_Data data){
    ChatDialog *log;

    if(!ChatDialogMap.contains(data.Data.UID)){
        log = new ChatDialog(data);
        this->connect(Recver,SIGNAL(SysFriendData(LinkC_Friend_Data)),log,SLOT(GetFriendData(LinkC_Friend_Data)));
        this->connect(log,SIGNAL(SendMessageToServer(LinkC_User_Request)),this,SLOT(SendMessageToServer(LinkC_User_Request)));
        log->show();
        ChatDialogMap.insert(data.Data.UID, log);
    }else{
        ChatDialogiterator = ChatDialogMap.find(data.Data.UID);
        log = ChatDialogiterator.value();
        log->show();
    }
    ((LUR*)package)->Action = USER_FRIEND_DATA;
    ((LUR*)package)->UID    = data.Data.UID;
    length = pack_message(USER_REQUEST,package,LUR_L,buffer);
    server.Send_msg(buffer,length,0);
}

void MainWindow::FriendLabelClicked(LinkC_Friend_Data data){
    ChatDialog *log;

    if(!ChatDialogMap.contains(data.Data.UID)){
        log = new ChatDialog(data);
        //  发送连接请求
        ((LUR*)package)->Action=USER_CHAT_REQUEST;
        ((LUR*)package)->UID   =data.Data.UID;
        length = pack_message(USER_REQUEST,package,LUR_L,buffer);
        server.Send_msg(buffer,length,0);
        //  end
        this->connect(Recver,SIGNAL(SysFriendData(LinkC_Friend_Data)),log,SLOT(GetFriendData(LinkC_Friend_Data)));
        this->connect(log,SIGNAL(SendMessageToServer(LinkC_User_Request)),this,SLOT(SendMessageToServer(LinkC_User_Request)));
        log->show();
        ChatDialogMap.insert(data.Data.UID, log);
    }else{
        ChatDialogiterator = ChatDialogMap.find(data.Data.UID);
        log = ChatDialogiterator.value();
        log->show();
    }
    ((LUR*)package)->Action = USER_FRIEND_DATA;
    ((LUR*)package)->UID    = data.Data.UID;
    length = pack_message(USER_REQUEST,package,LUR_L,buffer);
//    server.Send_msg(buffer,length,0);SYS_ACTION_STATUS,data,LSS_L,buffer;
}

void MainWindow::SLOT_RecvFriendList(void *data, int count){
    area->clear();
    area->setFriendCount(count);                       // save Friend count
    LinkC_Friend_Data *ffb = new LinkC_Friend_Data[area->FriendCount()];    // new memory
    memcpy(ffb,data,area->FriendCount() * sizeof(LinkC_Friend_Data));  // Save Friend Data

    int i;
    for(i=0;i<area->FriendCount();i++)
        area->AddFriendToLayout(ffb[i]);
    delete (char *)data;
    LinkC_Debug("Refresh Friend List\t= [SUCCESS]");
}

void MainWindow::UserMessage(struct LinkC_User_Message_t Message){
    if(Message.Action == USER_CHAT){
        char tmp[128];
        snprintf(tmp,127,"Your Friend[Whose UID is %d] Wants to chat with you",Message.SrcUID);
        int answer = QMessageBox::question(0,"QUESTION",tr(tmp),QMessageBox::Yes|QMessageBox::No);
        if(answer == QMessageBox::Yes){
            ChatWith(area->GetFriendDataByUID(Message.SrcUID));
        }
    }
    else if(Message.Action == USER_CONNECT_READY){
        if(!ChatDialogMap.contains(Message.SrcUID)){
        }else{
            ChatDialog *log;
            ChatDialogiterator = ChatDialogMap.find(Message.SrcUID);
            log = ChatDialogiterator.value();
            log->P2PConnectReady();
        }
    }
    else{
    }
}

void MainWindow::SysActionStatus(LinkC_Sys_Status status){
    if(status.Status == 0) return;
}

void MainWindow::SendMessageToServer(LinkC_User_Request Message){
    length = pack_message(USER_MESSAGE,(void *)&Message,LUM_L,package);
    server.Send_msg(package,length,0);
}

void MainWindow::SLOT_Quit(){
    this->close();
}

void MainWindow::SLOT_Refresh_User_Info(){
    printf("Here!\n");
    ((LUR*)package)->Action = USER_DATA_REQUEST;
    length = pack_message(USER_REQUEST,package,LUR_L,buffer);
    server.Send_msg(buffer,length,0);
}

void MainWindow::setUserData(LinkC_User_Data Data){
    User_Data = Data;
}

void MainWindow::SLOT_Refresh_Friend_List(){
    ((LUR *)package)->Action = USER_FRIEND_DATA;
    ((LUR *)package)->UID = 0;
    length = pack_message(USER_REQUEST,package,LUR_L,buffer);
    server.Send_msg(buffer,length,0);     // Send for Getting Friend Data
}
