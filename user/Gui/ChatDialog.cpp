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

ChatDialog::ChatDialog(LinkC_Friend_Data _MyFriend, QWidget *parent)
    :QWidget(parent){
    memcpy((void *)&MyFriend,(void *)&_MyFriend,sizeof(LinkC_Friend_Data));
    SendButton = new QPushButton(this);
    QuitButton = new QPushButton(this);
    Layout = new QVBoxLayout(this);
    History    = new ChatHistoryView(MyFriend.name);
    Input = new QTextEdit;
    peer    = new p2p_client;
    this->resize(300,300);

    char title_tmp[20];
    sprintf(title_tmp,"[%s] OFFLINE",MyFriend.name);
    QString Title(title_tmp);

    SendButton->setText(tr("Send"));
    SendButton->show();
    SendButton->setEnabled(false);
    QuitButton->hide();
    this->connect(SendButton,SIGNAL(clicked()),this,SLOT(Send()));
    this->connect(this,SIGNAL(StartP2PConnecting()),peer,SLOT(ConnectToPeer()));
    this->connect(peer,SIGNAL(ConnectToPeerDone(bool)),this,SLOT(P2PConnectDone(bool)));
    this->connect(peer,SIGNAL(AcceptReady()),this,SLOT(ReadyToAccept()));

    setWindowTitle(Title);

    Input->setText(tr("Input"));

    Layout->addWidget(History,2);
    Layout->addWidget(Input,1);
    Layout->addSpacing(25);

}

ChatDialog::~ChatDialog(){

}

void ChatDialog::ReadyToAccept(){
    LinkC_User_Request Message;
    Message.Action = USER_CONNECT_READY;
    Message.UID    = MyFriend.UID;
    emit SendMessageToServer(Message);  // to Tell peer that you have been ready for this P2P Connect
}

int ChatDialog::ConnectToPeer(void){
    return 0;
}

void ChatDialog::resizeEvent(QResizeEvent *){
    SendButton->setGeometry(this->width()-80,this->height()-30,70,25);
}

int ChatDialog::Send(void){
    QString str;
    QByteArray translate;
    str = Input->toPlainText();
    if (str == "")
        return 0;
    char buffer[STD_PACKAGE_SIZE];
    translate = str.toUtf8();
    int tmp = pack_message(USER_CHAT_MESSAGE,translate.data(),strlen(translate.data()),buffer);
    peer->GetCsocket().Send_msg(buffer,tmp,0);
    History->AddChatMessage(str,tr("Me"));
    Input->setText(tr(""));
    return 0;
}

void ChatDialog::GetFriendData(LinkC_Friend_Data Data){
    if(Data.UID != MyFriend.UID)    return;
    peer->SetDestIP(Data.ip);
    MyFriend=Data;
    char title_tmp[32];
    QString Title;
    if(Data.status == STATUS_ONLINE){
        if(peer->IsPeerConnected() == false){
            emit StartP2PConnecting();
            sprintf(title_tmp,"[%s] CONNECTING",MyFriend.name);
        }else
            sprintf(title_tmp,"[%s] ONLINE",MyFriend.name);
        Title = title_tmp;
        this->setWindowTitle(Title);
    }
}

void ChatDialog::P2PConnectDone(bool status){
    if(status == true){
        Recver = new UDP_MessageRecver(peer->GetCsocket());
        Recver->start();
        this->connect(Recver,SIGNAL(HeartBeats()),this,SLOT(ComeAHeartBeats()));
        this->connect(Recver,SIGNAL(RecvedP2PMessage(QString)),this,SLOT(RecvedP2PMessage(QString)));
        HeartBeater = new HeartBeats(peer->GetCsocket());
        HeartBeater->start();
        char title_tmp[32];
        sprintf(title_tmp,"[%s] CONNECTED",MyFriend.name);
        this->setWindowTitle(tr(title_tmp));
        this->SendButton->setEnabled(true);
    }else
        printf("Connect Error!\n");
}

void ChatDialog::ComeAHeartBeats(){

}

void ChatDialog::RecvedP2PMessage(QString Message){
    History->AddChatMessage(Message);
}

void ChatDialog::P2PConnectReady(){
    peer->inDirectConnectStart();
}


ChatHistoryView::ChatHistoryView(char *Name,QWidget *parent):
    QWidget(parent){
    MessageCount = 0;
    MessageBase  = new QWidget(this);
    MessageLayout= new QVBoxLayout;
    List         = new QScrollArea(this);

//    MessageBase->setLayout(MessageLayout);
    MessageBase->resize(1,1);
    List->setWidget(MessageBase);

    FriendName = Name;

}

int ChatHistoryView::GetMessageCount(){return MessageCount;}

void ChatHistoryView::resizeEvent(QResizeEvent *){
    List->resize(this->width(),this->height());
    MessageBase->resize(List->width()-15,_MESSAGE_HISTORY_HEIGTH*MessageCount);
}

void ChatHistoryView::AddChatMessage(QString Msg){
    QWidget *Histroy    = new QWidget(MessageBase);
    QLabel  *L1         = new QLabel(Histroy);
    QLabel  *L2         = new QLabel(Histroy);
    L1->setGeometry(5,0,500,15);
    L2->setGeometry(10,15,500,15);
    L1->setText(FriendName);
    L2->setText(Msg);
    Histroy->resize(this->width()-15,_MESSAGE_HISTORY_HEIGTH);
    MessageCount++;
    MessageBase->resize(List->width()-15,_MESSAGE_HISTORY_HEIGTH*MessageCount);
    Histroy->setGeometry(0,_MESSAGE_HISTORY_HEIGTH*(MessageCount-1),500,_MESSAGE_HISTORY_HEIGTH);
    Histroy->show();
}

void ChatHistoryView::AddChatMessage(QString Msg, QString Name){
    QWidget *Histroy    = new QWidget(MessageBase);
    QLabel  *L1         = new QLabel(Histroy);
    QLabel  *L2         = new QLabel(Histroy);
    L1->setGeometry(5,0,500,15);
    L2->setGeometry(10,15,500,15);
    L1->setText(Name);
    L2->setText(Msg);
    Histroy->resize(this->width()-15,_MESSAGE_HISTORY_HEIGTH);
    MessageCount++;
    MessageBase->resize(List->width()-15,_MESSAGE_HISTORY_HEIGTH*MessageCount);
    Histroy->setGeometry(0,_MESSAGE_HISTORY_HEIGTH*(MessageCount-1),500,_MESSAGE_HISTORY_HEIGTH);
    Histroy->show();
}
