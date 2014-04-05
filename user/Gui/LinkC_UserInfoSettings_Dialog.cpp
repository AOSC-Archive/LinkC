/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 5, 2014
 */
#include "LinkC_UserInfoSettings_Dialog.h"

LinkC_UserInfoSettings_Dialog::LinkC_UserInfoSettings_Dialog(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(450,450);
    this->setMaximumSize(800,600);
    this->resize(500,500);
    this->setWindowTitle(tr("User Informaction Settings"));
}

void LinkC_UserInfoSettings_Dialog::resizeEvent(QResizeEvent *){

}
