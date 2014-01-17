#include "LinkC_GUI.h"
#include "linkc_types.h"
#include "p2p_client.h"
#include "linkc_types.h"
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <QString>

#define MAX_ERROR   20

int i;
QString str;
QByteArray translate;

ChatDialog::ChatDialog(struct friend_data MyFriend, QWidget *parent)
    :QWidget(parent){
/*    MessageSize = sizeof(struct message_t);
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
    SendButton->setEnabled(false);
    QuitButton->hide();

    setWindowTitle(Title);
    History->setText(tr("History"));
    History->setReadOnly(TRUE);
    Input->setText(tr("Input"));

    Layout->addWidget(History,2);
    Layout->addWidget(Input,1);
    Layout->addSpacing(25);


    timer = new QTimer(this);
    timer->setInterval(5000);
    connect(timer, SIGNAL(timeout()),this, SLOT(HeartBeats()) );; // 5秒单触发定时器
    connect(SendButton,SIGNAL(clicked()),this,SLOT(Send()));
    timer->start();
    SendButton->setEnabled(true);
    Recver();*/
}

ChatDialog::~ChatDialog(){

}

int ChatDialog::ConnectToPeer(void){
    peer.Set_IP("127.0.0.1");
    peer.Set_Port(2342);
    peer.SetDestIP(MyFriend.ip);
    return peer.inDirectConnect();
}

void ChatDialog::resizeEvent(QResizeEvent *){
    SendButton->setGeometry(this->width()-80,this->height()-30,70,25);
}

int ChatDialog::Send(void){
/*    str = Input->toPlainText();
    if (str == "")
        return 0;
    message_send.header = MESSAGE;
    message_send.check_time = time(0);
    translate = str.toLatin1();
    sprintf(message_send.message,"%s",translate.data());
    for (i=0;i<5;i++)
        peer.Send_msg((void *)&message_send,MessageSize,MSG_DONTWAIT);*/
    return 0;
}

int ChatDialog::HeartBeats(){
/*    message_send.header = HEARTBEAT;
    for(i=0;i<5;i++)
        peer.Send_msg((void *)&message_send,MessageSize,MSG_DONTWAIT);*/
    return 0;
}

void ChatDialog::Recver(void){
/*    int error=0;
    time_t check_time = 0;
    while(1){
        if(error > MAX_ERROR)   // If There were so many errors has been coused
        {
            History->setText(tr("Peer Has Disconnected!"));
            break;
        }
        if(peer.Recv_msg((void *)&message_recv,MessageSize,0) <= 0)
        {
            error++;
            continue;
        }
        if (check_time == message_recv.check_time)   continue;
        switch (message_recv.header)
        {
            case MESSAGE:                   // if the message's Header is MESSAGE
            {
                check_time = message_recv.check_time;
                History->setText(tr("Peer : "));
                History->setText(message_recv.message);
                break;
            }
            case HEARTBEAT:                 // if the message's Header is HEARTBEAT
            {
                break;
            }
        }
        error = 0;
    }*/
}
