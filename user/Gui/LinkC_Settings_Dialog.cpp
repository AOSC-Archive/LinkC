/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 5, 2014
 */
#include "LinkC_Settings_Dialog.h"

LinkC_Settings_Dialog::LinkC_Settings_Dialog(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumSize(450,450);
    this->setMaximumSize(800,600);
    this->resize(500,500);
    this->setWindowTitle(tr("Generally Settings"));
}

LinkC_Settings_Dialog::~LinkC_Settings_Dialog()
{
}

void LinkC_Settings_Dialog::resizeEvent(QResizeEvent *){
}
