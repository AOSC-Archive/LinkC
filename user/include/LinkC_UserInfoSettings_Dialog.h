/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 5, 2014
 */
#ifndef LINKC_USERINFOSETTINGS_DIALOG_H
#define LINKC_USERINFOSETTINGS_DIALOG_H

#include <QWidget>

class LinkC_UserInfoSettings_Dialog : public QWidget
{
    Q_OBJECT
public:
    explicit LinkC_UserInfoSettings_Dialog(QWidget *parent = 0);
    void resizeEvent(QResizeEvent *);

signals:

public slots:

};

#endif // LINKC_USERINFOSETTINGS_DIALOG_H
