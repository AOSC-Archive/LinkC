#include "MainWindow.h"
#include "../NetWork/csocket.h"
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

#include "../include/data_type.h"
#include "../include/LinkC_Label.h"
#include "../include/LinkC_GUI.h"

#define MAXBUF 1024
char buffer[MAXBUF];
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
    setWindowTitle("Main_Window");      // 标题
    Connection_state = NetworkInit();   // 初始化网络
    if(Connection_state == -1)exit(0);  // 如果链接失败，退出
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
	server.Send_msg(LINKC_QUIT,MSG_DONTWAIT);
	printf ("Debug >> Done!\n");
}

void MainWindow::test_slot(int i){
    printf("%d\n",i);
}

int MainWindow::NetworkInit(void){
	int i;
    server.Set_IP("117.59.12.19");
    server.Debug_Csocket_IP();
	server.Set_Port(2341);
	server.Debug_Csocket_Port();
	i = server.ConnectToServer();
	if (i == 0)
	{
		QMessageBox::warning(0,"Waring","Connection refused!",QMessageBox::Yes);
		return(-1);
	}
	server.Debug_Csocket_Sockfd();
	server.Recv_msg(buffer,MSG_WAITALL);
	if (!strncasecmp(buffer,LINKC_OK,MAXBUF))printf ("Debug >> Connect\t= [Successful]\n");
	else if(!strncasecmp(buffer,LINKC_FAILURE,MAXBUF))printf ("Debug >> Connect\t= [Faliure]\n");
	server.cls_buf(buffer,MAXBUF);
	return 1;
}

int MainWindow::Login(){

	struct login_data st;
	int i;
	while(1){

		i = s->exec();
		if (i == 1){

			i = s->GetLoginData(st);
			if (i == -1)continue;
			server.Send_msg(LINKC_LOGIN,MSG_DONTWAIT);
			server.Send_msg((void *)&st,MSG_WAITALL);
			server.cls_buf(buffer,1024);
			i = server.Recv_msg(buffer,MSG_WAITALL);
			if (!strncasecmp(buffer,LINKC_OK,MAXBUF)){

				printf ("Debug >> Login\t\t= [Successful]\n");
				server.cls_buf(buffer,MAXBUF);
				this->show();
				return 1;
			}
			else if (!strncasecmp(buffer,LINKC_FAILURE,MAXBUF)){
				QMessageBox::warning(0,"Waring","Login Faliure!",QMessageBox::Yes);
			}
			else if (!strncasecmp(buffer,LINKC_TRY_SO_MANY,MAXBUF)) {
				QMessageBox::warning(0,"Waring","You Try So Much Times!",QMessageBox::Yes);
			}
			printf ("Debug >> Login\t\t= [Faliure]\n");
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
    server.Send_msg(LINKC_GET_FRIENDS,MSG_WAITALL);     // Send for Getting Friend Data
    server.Recv_msg(buffer,MSG_WAITALL);                // recv state
    printf("State = %s\n",buffer);                      // debug
    if (!strncasecmp(buffer,LINKC_NO_FRIEND,MAXBUF))    // if This Has no friend
            return 0;
    server.Recv_msg(buffer,MSG_WAITALL);                // recv Friend count
    printf("I Have %s Friend(s)\n",buffer);             // debug
    area->setFriendCount(buffer);                       // save Friend count
    friend_data *ffb = new friend_data[area->FriendCount()];    // new memory
    server.Recv_msg(buffer,MSG_WAITALL);                // recv Friend Data
    memcpy(ffb,buffer,area->FriendCount() * sizeof(friend_data));  // Save Friend Data
    printf("Name = %s\n",ffb[0].name);                  // Debug
    server.Send_msg(LINKC_GET_FRIEND,MSG_WAITALL);
    sprintf(buffer,"%d",ffb[0].UID);
    server.Send_msg(buffer,MSG_WAITALL);
    friend_data *data = new friend_data;
    server.Recv_msg(buffer,MSG_WAITALL);                // recv Friend Data
    memcpy(data,buffer,sizeof(friend_data));  // Save Friend Data
    printf("%s\n",data->ip);
    area->AddFriendToLayout(ffb[0]);

    return 1;
}

void MainWindow::check(){
    printf ("ALl Right!\n");
}

void MainWindow::ChatWith(int UID){
    ChatDialog *log;
    if(!ChatDialogMap.contains(UID)){
        log = new ChatDialog(UID);
        log->show();
        ChatDialogMap.insert(UID, log);
    }else{
        tmp = ChatDialogMap.find(UID);
        log = tmp.value();
        log->show();
    }
}

//#####################################################33
/*ChatDialog::ChatDialog(QWidget *parent)
    :QDialog(parent){
    resize(200,200);
    show();
}
*/
