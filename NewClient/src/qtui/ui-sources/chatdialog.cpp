#include "chatdialog.h"
#include "linkc_error.h"
#include "linkc_package_ctl.h"
#include "linkc_package.h"
#include "linkc_def.h"
#include "linkc_netowrk_UDP_system.h"
#include <QTextEdit>
#include <QMessageBox>


ChatDialog::ChatDialog(UserData _MyFriend, QWidget *parent)
    :QWidget(parent){
    memcpy((void *)&MyFriend,(void *)&_MyFriend,sizeof(UserData));
    SendButton  = new QPushButton(this);
    QuitButton  = new QPushButton(this);
    ConnectButton=new QPushButton(this);
    Layout      = new QVBoxLayout(this);
    History     = new ChatHistoryView(MyFriend.UserName);
    Input       = new QTextEdit;
    Socket      = new UDP_Socket;
    Buffer      = new char[STD_PACKAGE_SIZE];
    Package     = new char[STD_PACKAGE_SIZE];
    this->resize(300,300);

    char title_tmp[20];
    sprintf(title_tmp,"[%s] OFFLINE",MyFriend.UserName);
    QString Title(title_tmp);

    SendButton->setText(tr("Send"));
    SendButton->show();
    ConnectButton->setText(tr("Connect"));
    ConnectButton->show();
    SendButton->setEnabled(false);
    QuitButton->hide();
    this->connect(SendButton,SIGNAL(clicked()),this,SLOT(Send()));
    this->connect(Input,SIGNAL(textChanged()),this,SLOT(InputEditChanged()));
    this->connect(this,SIGNAL(DoP2PConnect(uint32_t)),Socket,SLOT(DoP2PConnect(uint32_t)));
    this->connect(ConnectButton,SIGNAL(clicked()),this,SLOT(StartConnect()));

    setWindowTitle(Title);

    Input->setText(tr("Input"));

    Layout->addWidget(History,2);
    Layout->addWidget(Input,1);
    Layout->addSpacing(25);

    this->show();
}

void ChatDialog::resizeEvent(QResizeEvent *){
    SendButton->setGeometry(this->width()-80,this->height()-30,70,25);
    ConnectButton->setGeometry(this->width()-270,this->height()-30,70,25);
}

void ChatDialog::Send(){
     memcpy(Buffer,Input->toPlainText().toUtf8().data(),Input->toPlainText().length());
     int Length = _Package(Buffer,Input->toPlainText().length(),P2P_DATA,Package);
     SendMessage(Socket->GetSockfd(),Package,Length,0);
}

void ChatDialog::InputEditChanged(){
    if(Input->toPlainText() == "")  SendButton->setDisabled(true);
    else                            SendButton->setDisabled(false);
}

void ChatDialog::StartConnect(){
    if(Socket->DoP2PConnect(MyFriend.IP) == LINKC_SUCCESS){
        Recver = new P2PMessageRecver(Socket->GetSockfd());
        this->connect(Recver,SIGNAL(MessageRecved(QString)),History,SLOT(ShowMessage(QString)));
        Recver->start();
    }

}

ChatDialog::~ChatDialog(){
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

void ChatHistoryView::ShowMessage(QString Msg){
    this->AddChatMessage(Msg,tr("Peer Said"));
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


P2PMessageRecver::P2PMessageRecver(int Socket, QThread *parent):
    QThread(parent){
    Sockfd =Socket;
    Buffer = new char[STD_PACKAGE_SIZE];
    Package =new char[STD_PACKAGE_SIZE];
}

P2PMessageRecver::~P2PMessageRecver(){
    delete (char*)Buffer;
    delete (char*)Package;
}

void P2PMessageRecver::run(){
    QString Recved;
    while(1){
        if(RecvMessage(Sockfd,Buffer,STD_PACKAGE_SIZE,0)==LINKC_FAILURE){
            LinkC_Debug("P2PMessageRecver:RecvMessage",LINKC_FAILURE);
        }else{
            bzero(Package,STD_BUFFER_SIZE);
            if(((PackageHeader*)Buffer)->MessageType != P2P_DATA)   continue;   //  忽略非P2P消息
            _UnPackage(Buffer,STD_PACKAGE_SIZE,Package);
            Recved = (char*)Package;
            emit MessageRecved(Recved);
        }
    }
}
