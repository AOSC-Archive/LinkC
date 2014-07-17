#include "chatdialog.h"
#include <QTextEdit>
#include <QMessageBox>


ChatDialog::ChatDialog(UserData _MyFriend, QWidget *parent)
    :QWidget(parent){
    memcpy((void *)&MyFriend,(void *)&_MyFriend,sizeof(UserData));
    SendButton  = new QPushButton(this);
    QuitButton  = new QPushButton(this);
    Layout      = new QVBoxLayout(this);
    History     = new ChatHistoryView(MyFriend.UserName);
    Input       = new QTextEdit;
    Socket      = new UDP_Socket;
    this->resize(300,300);

    char title_tmp[20];
    sprintf(title_tmp,"[%s] OFFLINE",MyFriend.UserName);
    QString Title(title_tmp);

    SendButton->setText(tr("Send"));
    SendButton->show();
    SendButton->setEnabled(false);
    QuitButton->hide();
    this->connect(SendButton,SIGNAL(clicked()),this,SLOT(Send()));
    this->connect(Input,SIGNAL(textChanged()),this,SLOT(InputEditChanged()));
    this->connect(this,SIGNAL(DoP2PConnect(uint32_t)),Socket,SLOT(DoP2PConnect(uint32_t)));

    setWindowTitle(Title);

    Input->setText(tr("Input"));

    Layout->addWidget(History,2);
    Layout->addWidget(Input,1);
    Layout->addSpacing(25);

    if(MyFriend.IP != 0){
        emit DoP2PConnect(MyFriend.IP);
    }
}

void ChatDialog::resizeEvent(QResizeEvent *){
    SendButton->setGeometry(this->width()-80,this->height()-30,70,25);
}

void ChatDialog::Send(){

}

void ChatDialog::InputEditChanged(){
    if(Input->toPlainText() == "")  SendButton->setDisabled(true);
    else                            SendButton->setDisabled(false);
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
