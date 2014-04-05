/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 5, 2014
 */
#include "LinkC_GUI.h"

SetupMenu::SetupMenu(QWidget *parent):
    QPushButton(parent){
    Menu                        = new QMenu;
    A_Quit                      = new QAction(tr("退出"),Menu);
    A_Refresh_User_Info         = new QAction(tr("刷新个人资料"),Menu);
    A_Refresh_Friend_List       = new QAction(tr("刷新好友列表"),Menu);
    A_Settings_Dialog           = new QAction(tr("设置"),Menu);
    A_UserInforSettings_Dialog  = new QAction(tr("个人资料"),Menu);
    this->setMenu(Menu);
    Menu->addAction(A_Quit);
    Menu->addAction(A_Refresh_User_Info);
    Menu->addAction(A_Refresh_Friend_List);
    Menu->addAction(A_Settings_Dialog);
    Menu->addAction(A_UserInforSettings_Dialog);

    this->connect(A_Quit,SIGNAL(triggered()),this,SLOT(SLOT_Quit()));
    this->connect(A_Refresh_User_Info,SIGNAL(triggered()),this,SLOT(SLOT_Refresh_User_info()));
    this->connect(A_Refresh_Friend_List,SIGNAL(triggered()),this,SLOT(SLOT_Refresh_Friend_List()));
    this->connect(A_Settings_Dialog,SIGNAL(triggered()),this,SLOT(SLOT_Settings_Dialog()));
    this->connect(A_UserInforSettings_Dialog,SIGNAL(triggered()),this,SLOT(SLOT_UserInfoSettings_Dialog()));
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

void SetupMenu::SLOT_Settings_Dialog(){
    emit SIG_Settings_Dialog();
}

void SetupMenu::SLOT_UserInfoSettings_Dialog(){
    emit SIG_UserInfoSettings_Dialog();
}
