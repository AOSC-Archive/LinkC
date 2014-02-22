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
    peer    = new p2p_client;
    this->resize(300,300);

    char title_tmp[20];
    sprintf(title_tmp,"[%s] OFFLINE",MyFriend.name);
    QString Title(title_tmp);

    SendButton->setText(tr("Send"));
    SendButton->show();
//    SendButton->setEnabled(false);
    QuitButton->hide();
    this->connect(SendButton,SIGNAL(clicked()),this,SLOT(Send()));
    this->connect(this,SIGNAL(StartP2PConnecting()),peer,SLOT(ConnectToPeer()));
    this->connect(peer,SIGNAL(ConnectToPeerDone(bool)),this,SLOT(P2PConnectDone(bool)));
    this->connect(Recver,SIGNAL(HeartBeats()),this,SLOT(ComeAHeartBeats()));

    setWindowTitle(Title);

    History->setText(tr("History"));
    History->setReadOnly(true);
    Input->setText(tr("Input"));

    Layout->addWidget(History,2);
    Layout->addWidget(Input,1);
    Layout->addSpacing(25);

    if(MyFriend.status == STATUS_ONLINE)
        History->setText(tr("ONLINE"));
    else
        History->setText(tr("OFFLINE"));
}

ChatDialog::~ChatDialog(){

}

int ChatDialog::ConnectToPeer(void){
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
    translate = str.toUtf8();
    int tmp = pack_message(USER_CHAT_MESSAGE,translate.data(),strlen(translate.data()),buffer);
    peer->GetCsocket().Send_msg(buffer,tmp,0);
    printf("CLIENT:MessageSended!\n");
    return 0;
}

void ChatDialog::GetFriendData(LinkC_Friend_Data Data){
    if(Data.UID != MyFriend.UID)    return;
    printf("Data Get!\nName = %s\n",Data.name);
    peer->SetDestIP(Data.ip);
    MyFriend=Data;
    int OldStatus = MyFriend.status;
    if(Data.status == STATUS_ONLINE){
        if(peer->IsPeerConnected() == false)
            emit StartP2PConnecting();
    }
    if(OldStatus == STATUS_OFFLINE){
        char title_tmp[20];
        sprintf(title_tmp,"[%s] ONLINE",MyFriend.name);
        QString Title(title_tmp);
        this->setWindowTitle(Title);
        History->setText(tr("ONLINE"));
    }
}

void ChatDialog::P2PConnectDone(bool status){
    if(status == true){
        printf("Chat Message Recver Started!\n");
        Recver = new UDP_MessageRecver(peer->GetCsocket());
        Recver->start();
        HeartBeater = new HeartBeats(peer->GetCsocket());
        HeartBeater->start();
    }else
        printf("Connect Error!\n");
}

void ChatDialog::ComeAHeartBeats(){
    printf("Heart Beats!\n");
}
