#include "../include/LinkC_GUI.h"

ChatDialog::ChatDialog(int _UID,QWidget *parent)
    :QWidget(parent){
    UID=_UID;
    SendButton  = new QPushButton(this);
    connect(SendButton,SIGNAL(clicked()),this,SLOT(Send()));
    QuitButton  = new QPushButton(this);
    Layout      = new QVBoxLayout(this);
    Input       = new QTextEdit;
    History     = new QTextEdit;
    this->resize(300,300);
    SendButton->setText(tr("Send"));
    SendButton->show();
    QuitButton->hide();

    setWindowTitle(tr("Chat With XXX"));
    History->setText(tr("History"));
    History->setReadOnly(TRUE);
    Input->setText(tr("Input"));

    Layout->addWidget(History,2);
    Layout->addWidget(Input,1);
    Layout->addSpacing(25);

}

ChatDialog::~ChatDialog(){

}

void ChatDialog::resizeEvent(QResizeEvent *){
    SendButton->setGeometry(this->width()-80,this->height()-30,70,25);
}

int ChatDialog::Send(void){
    return 0;
}
