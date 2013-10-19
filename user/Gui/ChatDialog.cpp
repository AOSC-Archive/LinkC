#include "../include/LinkC_GUI.h"
#include "data_type.h"
#include "p2p_client.h"
#include <QString>
#include <stdio.h>

ChatDialog::ChatDialog(struct friend_data MyFriend, QWidget *parent)
    :QWidget(parent){
                printf("%d\n",MyFriend.state);
    SendButton = new QPushButton(this);
    connect(SendButton,SIGNAL(clicked()),this,SLOT(Send()));
    QuitButton = new QPushButton(this);
    Layout = new QVBoxLayout(this);
    Input = new QTextEdit;
    History = new QTextEdit;
    this->resize(300,300);
    SendButton->setText(tr("Send"));
    SendButton->show();
    QuitButton->hide();

    setWindowTitle(tr(MyFriend.name));
    History->setText(tr("History"));
    History->setReadOnly(TRUE);
    Input->setText(tr("Input"));

    Layout->addWidget(History,2);
    Layout->addWidget(Input,1);
    Layout->addSpacing(25);

    //NetWork Init
    if(MyFriend.state == STATE_OFFLINE){
        History->setText(tr("Friend is Offline!\nLinkC NOT Support OFFLINE MESSAGE PUSHING"));
    }else{
    p2p_client peer;
    peer.Set_IP("127.0.0.1");
    peer.Set_Port(2342);
    peer.SetDestIP(MyFriend.ip);
    }


}

ChatDialog::~ChatDialog(){

}

void ChatDialog::resizeEvent(QResizeEvent *){
    SendButton->setGeometry(this->width()-80,this->height()-30,70,25);
}

int ChatDialog::Send(void){
    return 0;
}
