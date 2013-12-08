#include "LinkC_Label.h"
#include <qt4/QtGui/QWidget>
#include <qt4/QtGui/QtGui>

LinkC_Label::LinkC_Label(QWidget *parent)
    :QLabel(parent){

}

void LinkC_Label::mousePressEvent(QMouseEvent *event){
    //如果单击了就触发clicked信号
    if (event->button() == Qt::LeftButton) {
        //触发clicked信号
        emit clicked();
    }
    else
    //将该事件传给父类处理
    QLabel::mousePressEvent(event);
}
