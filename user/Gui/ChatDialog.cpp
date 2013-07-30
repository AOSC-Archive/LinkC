#include "../include/LinkC_GUI.h"

ChatDialog::ChatDialog(QWidget *parent)
    :QWidget(parent){
    Layout = new QVBoxLayout(this);
    Input  = new QDockWidget(this);


    setWindowTitle(tr("Chat With XXX"));
    QTextEdit *te=new QTextEdit(this);
    te->setText(tr("History"));
    Layout->addWidget(te);
    //设置停靠窗口1
    Input->setFeatures(QDockWidget::DockWidgetMovable);
    Input->setAllowedAreas(Qt::BottomDockWidgetArea);
    QTextEdit *te1=new QTextEdit();
    te1->setText(tr("Input"));
    Input->setWidget(te1);

    Layout->addWidget(Input);

}
