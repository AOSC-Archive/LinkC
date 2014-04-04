/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 4, 2014
 */
#include "LinkC_Label.h"

LinkC_Label::LinkC_Label(QWidget *parent)
    :QLabel(parent){

}

LinkC_Label::LinkC_Label(QString Title, QWidget *parent)
    :QLabel(parent){
    this->setText(Title);
}

void LinkC_Label::mousePressEvent(QMouseEvent *event){
    //如果单击了就触发clicked信号
    if (event->button() == Qt::LeftButton) {
        //触发clicked信号
        emit clicked();
    }
    else{
    //将该事件传给父类处理
    QLabel::mousePressEvent(event);
    }
}
