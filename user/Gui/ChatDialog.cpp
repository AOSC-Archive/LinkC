#include "../include/LinkC_GUI.h"
#include "data_type.h"
#include "p2p_client.h"
#include "data_type.h"
#include <QString>
#include <stdio.h>

ChatDialog::ChatDialog(struct friend_data MyFriend, QWidget *parent)
    :QWidget(parent){
    SendButton = new QPushButton(this);
    connect(SendButton,SIGNAL(clicked()),this,SLOT(Send()));
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

    //NetWork Init
    if(MyFriend.state == STATE_OFFLINE){
        History->setText(tr("Friend is Offline!\nLinkC NOT Supports OFFLINE MESSAGE PUSHING"));
        return;
    }
    if (ConnectToPeer() < 0){
        History->setText(tr("Can Not Connect to Your Friend!\nTry Again..."));
        if ((ConnectToPeer()) == 0)
            History->setText(tr("Connect Success"));
        else return;
    }
    SendButton->setEnabled(true);
//    this->connect()
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

    return 0;
}
