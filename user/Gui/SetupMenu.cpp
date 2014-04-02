#include "LinkC_GUI.h"

SetupMenu::SetupMenu(QWidget *parent):
    QPushButton(parent){
    Menu                = new QMenu;
    A_Quit              = new QAction(tr("退出"),Menu);
    A_Refresh_User_Info = new QAction(tr("刷新个人资料"),Menu);
    A_Refresh_Friend_List=new QAction(tr("刷新好友列表"),Menu);
    this->setMenu(Menu);
    Menu->addAction(A_Quit);
    Menu->addAction(A_Refresh_User_Info);
    Menu->addAction(A_Refresh_Friend_List);

    this->connect(A_Quit,SIGNAL(triggered()),this,SLOT(SLOT_Quit()));
    this->connect(A_Refresh_User_Info,SIGNAL(triggered()),this,SLOT(SLOT_Refresh_User_info()));
    this->connect(A_Refresh_Friend_List,SIGNAL(triggered()),this,SLOT(SLOT_Refresh_Friend_List()));
}

SetupMenu::~SetupMenu(){

}

void SetupMenu::SLOT_Quit(){
    emit SIG_Quit();
}

void SetupMenu::SLOT_Refresh_User_info(){
    emit SIG_Refresh_User_Info();
}

void SetupMenu::SLOT_Refresh_Friend_List(){
    emit SIG_Refresh_Friend_List();
}
