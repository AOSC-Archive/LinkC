#ifndef LINKC_LABEL_H
#define LINKC_LABEL_H

#include <qt4/QtGui/QLabel>
#include <qt4/QtCore/QObject>

class LinkC_Label : public QLabel{
    Q_OBJECT
public:
    explicit LinkC_Label(QWidget *parent = 0);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
signals:
    void clicked();

public slots:

};

#endif
