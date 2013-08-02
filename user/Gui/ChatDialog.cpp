#include "../include/LinkC_GUI.h"

ChatDialog::ChatDialog(QWidget *parent)
    :QWidget(parent){
    Layout      = new QVBoxLayout(this);
    Input       = new QDockWidget(this);
    Send        = new QPushButton(this);
    this->resize(300,300);
    Send->setGeometry(this->width(),this->height(),100,100);

    Send->setText(tr("Send"));
    Send->show();

    setWindowTitle(tr("Chat With XXX"));
    QDockWidget *sl = new QDockWidget(this);
    QTextEdit *te=new QTextEdit(this);
        sl->setWidget(te);
        sl->setFeatures(QDockWidget::DockWidgetMovable);
    te->setText(tr("History"));
    Layout->addWidget(sl,2);
    //设置停靠窗口1
    Input->setFeatures(QDockWidget::DockWidgetMovable);
    Input->setAllowedAreas(Qt::BottomDockWidgetArea);
    QTextEdit *te1=new QTextEdit();
    te1->setText(tr("Input"));

    Input->setWidget(te1);
    Layout->addWidget(Input,1);
    Layout->addSpacing(25);




}

void ChatDialog::resizeEvent(QResizeEvent *){
    Send->setGeometry(this->width()-80,this->height()-30,70,25);
}
