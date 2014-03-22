#include "LinkC_GUI.h"

SetupMenu::SetupMenu(QWidget *parent):
    QPushButton(parent){
    Menu                = new QMenu;
    A_Quit              = new QAction(tr("退出"),Menu);
    A_Refresh_User_Info = new QAction(tr("刷新个人资料"),Menu);
    this->setMenu(Menu);
    Menu->addAction(A_Quit);
    Menu->addAction(A_Refresh_User_Info);

    this->connect(A_Quit,SIGNAL(triggered()),this,SLOT(SLOT_Quit()));
    this->connect(A_Refresh_User_Info,SIGNAL(triggered()),this,SLOT(SLOT_Refresh_User_info()));
}

SetupMenu::~SetupMenu(){

}

void SetupMenu::SLOT_Quit(){
    emit SIG_Quit();
}

void SetupMenu::SLOT_Refresh_User_info(){
    emit SIG_Refresh_User_Info();
}
