/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 4, 2014
 */
#ifndef LINKC_LABEL_H
#define LINKC_LABEL_H

#include <QLabel>
#include <QObject>
#include <QMouseEvent>

class LinkC_Label : public QLabel{
    Q_OBJECT
public:
    explicit LinkC_Label(QWidget *parent = 0);
    explicit LinkC_Label(QString Title,QWidget *parent = 0);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
signals:
    void clicked();

public slots:

};

#endif
