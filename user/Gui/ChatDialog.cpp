#include "LinkC_GUI.h"
#include "linkc_types.h"
#include "p2p_client.h"
#include "linkc_types.h"
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <QString>
#include <unistd.h>

#define MAX_ERROR   20

int i;
QString str;
QByteArray translate;

ChatDialog::ChatDialog(LinkC_Friend_Data _MyFriend, QWidget *parent)
    :QWidget(parent){
    memcpy((void *)&MyFriend,(void *)&_MyFriend,sizeof(LinkC_Friend_Data));
    SendButton = new QPushButton(this);
    QuitButton = new QPushButton(this);
    Layout = new QVBoxLayout(this);
    Input = new QTextEdit;
    History = new QTextEdit;
    this->resize(300,300);

    char title_tmp[20];
    sprintf(title_tmp,"Chat With %s",MyFriend.name);
    QString Title(title_tmp);

    SendButton->setText(tr("Send"));
    SendButton->show();
//    SendButton->setEnabled(false);
    QuitButton->hide();
    connect(SendButton,SIGNAL(clicked()),this,SLOT(Send()));

    setWindowTitle(Title);

    History->setText(tr("History"));
    History->setReadOnly(true);
    Input->setText(tr("Input"));

    Layout->addWidget(History,2);
    Layout->addWidget(Input,1);
    Layout->addSpacing(25);

    if(MyFriend.status == STATUS_ONLINE){
        History->setText(tr("ONLINE"));
        this->ConnectToPeer();
        pid_t pid;
        pid = fork();
        if(pid == 0)
            this->Recver();
    }
    else
        History->setText(tr("OFFLINE"));
}

ChatDialog::~ChatDialog(){

}

int ChatDialog::ConnectToPeer(void){
    peer.Set_IP("127.0.0.1");
    peer.Debug_Csocket_IP();
    peer.Set_Port(2342);
    peer.Debug_Csocket_Port();
    peer.SetDestIP(MyFriend.ip);
    if(peer.WaitPeer() == LINKC_FAILURE){
        History->setText(tr("ERROR"));
        return -1;
    }
    if(peer.Is_server() == 1)
        peer.inDirectAccept();
    else
        peer.inDirectConnect();
    return 0;
}

void ChatDialog::resizeEvent(QResizeEvent *){
    SendButton->setGeometry(this->width()-80,this->height()-30,70,25);
}

int ChatDialog::Send(void){
    str = Input->toPlainText();
    if (str == "")
        return 0;
    char buffer[STD_PACKAGE_SIZE];
    printf("Hei!\n");
    translate = str.toLatin1();
    int tmp = pack_message(USER_MESSAGE,translate.data(),strlen(translate.data()),buffer);
    peer.Send_msg(buffer,tmp,0);
    return 0;
}

int ChatDialog::HeartBeats(){
/*    message_send.header = HEARTBEAT;
    for(i=0;i<5;i++)
        peer.Send_msg((void *)&message_send,MessageSize,MSG_DONTWAIT);*/
    return 0;
}

void ChatDialog::Recver(void){
    char buffer[STD_PACKAGE_SIZE];
    char buf[STD_PACKAGE_SIZE];
    while(1){
        bzero(buffer,STD_PACKAGE_SIZE);
        peer.UDP_Recv(buffer,STD_PACKAGE_SIZE,0);
        unpack_message(buffer,buf);
        printf("Peer Said = %s\n",buf);
        sleep(1);
    }
}
