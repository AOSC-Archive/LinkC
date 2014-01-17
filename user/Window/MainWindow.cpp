#include "MainWindow.h"
#include "Csocket.h"
#include "linkc_types.h"
#include "LinkC_Label.h"
#include "LinkC_GUI.h"
#include "linkc_network_protocol.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <qt4/Qt/qmessagebox.h>
#include <qt4/Qt/qstring.h>
#include <qt4/Qt/qgroupbox.h>
#include <qt4/Qt/qtoolbox.h>
#include <qt4/Qt/qwidget.h>
#include <qt4/Qt/qpushbutton.h>
#include <qt4/QtGui/QtGui>
#include <qt4/QtGui/QListView>
#include <qt4/QtGui/QMainWindow>


char buffer[MAXBUF];
int lenth;
int flag;
LoginWindow *s;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent){
    Top         = new QWidget(this);    // 顶部
    MainWidget  = new QWidget(this);    // 中部
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
    this->setCentralWidget(MainWidget); // 设置主widget
    MainWidget->setLayout(layout);      // 设置主layout
    layout->addLayout(MainLayout,0,0,1,1);
    this->setMaximumSize(300,500);
    this->setMinimumSize(150,200);
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
        this->connect(head, SIGNAL(clicked()), this, SLOT(check()));
        this->connect(area, SIGNAL(ChatTo(int)),this, SLOT(ChatWith(int)));
        head->show();
//############顶部初始化完毕############

//############初始化中间部分############
    MainLayout->addSpacing(50);
    MainLayout->addWidget(tab);
    tab->addTab(area,QString("Friend"));

//############登录####################
    Login();        //登录
    delete s;
    InitFriendList();


}

MainWindow::~MainWindow(){
//    lenth = pack_message(EXIT,NULL,0,buffer);
//    server.Send_msg(buffer,lenth,MSG_DONTWAIT);
    printf ("Debug >> Main_Window\t= [EXITED]\n");
}

void MainWindow::test_slot(int i){
    printf("%d\n",i);
}

int MainWindow::NetworkInit(void){
	int i;
    server.Set_IP("127.0.0.1");
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
    lenth = pack_message(CONNECTION,package,0,buffer);
    server.Send_msg(buffer,lenth,0);
    lenth = server.Recv_msg(buffer,STD_PACKAGE_SIZE,0);
    flag = check_message(buffer,lenth);
    if (flag == SYS_ACTION_STATUS){
        unpack_message(buffer,package);
        if(((LinkC_Sys_Status *)package)->Action == CONNECTION){
            if(((LinkC_Sys_Status *)package)->Status == LINKC_SUCCESS){
                printf ("Debug >> Connect\t= [Success]\n");
                return 0;
            }
            else if(((LinkC_Sys_Status *)package)->Status == LINKC_FAILURE){
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
            lenth = pack_message(LOGIN,(void *)&st,LUL_L,package);
            server.Send_msg(package,lenth,0);
            lenth = server.Recv_msg(buffer,STD_PACKAGE_SIZE,0);
            flag = check_message(buffer,lenth);
            if (flag == SYS_ACTION_STATUS){
                unpack_message(buffer,package);
                if(((LinkC_Sys_Status *)package)->Action == LOGIN){
                    if(((LinkC_Sys_Status *)package)->Status == LINKC_SUCCESS){
                        printf ("Debug >> Login\t\t= [Success]\n");
                        this->show();
                        return 0;
                    }
                    else if(((LinkC_Sys_Status *)package)->Status == LINKC_FAILURE){
                        printf ("Debug >> Login\t\t= [Failure]\n");
                        QMessageBox::warning(0,"Waring","Login Faliure!",QMessageBox::Yes);
                        continue;
                    }
                    else if(((LinkC_Sys_Status *)package)->Status == LINKC_LIMITED){
                        QMessageBox::warning(0,"Waring","Please Try later!",QMessageBox::Yes);
                        printf ("Debug >> Login\t\t= [Limited]\n");
                        exit(0);
                    }
                }
            }
            printf("Debug >> Login\t\t= [Package Broken]\n");
            exit(0);
		}
		if (i == -1)exit(0);
	}
}

void MainWindow::resizeEvent(QResizeEvent *){
    Top->setGeometry(0,0,this->width(),50);
}

void MainWindow::closeEvent(QCloseEvent *){
    for ( tmp = ChatDialogMap.begin(); tmp != ChatDialogMap.end(); ++tmp )
                tmp.value()->~ChatDialog();
}

int MainWindow::InitFriendList(){
    int flag;
    ((LinkC_User_Request *)package)->Action = USER_FRIEND_DATA;
    ((LinkC_User_Request *)package)->Flag = 0;
    lenth = pack_message(USER_REQUEST,package,LUR_L,buffer);
    server.Send_msg(buffer,lenth,0);     // Send for Getting Friend Data
    lenth = server.Recv_msg(buffer,0);                // recv state
    flag = check_message(buffer,lenth);
    if(flag == SYS_ACTION_STATUS){
        unpack_message(buffer,package);
        if(((LinkC_Sys_Status *)package)->Action == USER_FRIEND_DATA){
            if(((LinkC_Sys_Status *)package)->Status == LINKC_SUCCESS)
                printf("Debug >> State\t\t= [Success]\n");
            else{
                if(((LinkC_Sys_Status *)package)->Status == LINKC_FAILURE)
                    printf("Debug >> State\t\t= [Failure]\n");
                else
                    printf("Debug >> Friend\t\t= [NULL]");
                return 0;
            }
        }
    }
    flag = non_std_m_message_recv(server.GetSockfd(),sizeof(friend_data),package);
    printf("Debug >> Friends Count\t= [%d]\n",flag);             // debug
    area->setFriendCount(flag);                       // save Friend count
    friend_data *ffb = new friend_data[area->FriendCount()];    // new memory
    memcpy(ffb,package,area->FriendCount() * sizeof(friend_data));  // Save Friend Data

    area->AddFriendToLayout(ffb[0]);

    return 0;
}

void MainWindow::check(){
    printf ("ALl Right!\n");
}

void MainWindow::ChatWith(int UID){
/*    ChatDialog *log;
    server.Send_msg(LINKC_GET_FRIEND,0);
    sprintf(buffer,"%d",UID);
    server.Send_msg(buffer,MSG_DONTWAIT);
    server.Recv_msg(buffer,0);
    memcpy((void *)&MyFriend,buffer,sizeof(MyFriend));
    if(!ChatDialogMap.contains(UID)){
        log = new ChatDialog(MyFriend);
        log->show();
        ChatDialogMap.insert(UID, log);
    }else{
        tmp = ChatDialogMap.find(UID);
        log = tmp.value();
        log->show();
    }*/
}
