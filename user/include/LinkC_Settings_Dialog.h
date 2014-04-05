/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： April 5, 2014
 */
#ifndef LINKC_SETTINGS_DIALOG_H
#define LINKC_SETTINGS_DIALOG_H

#include <QWidget>

namespace Ui {
class LinkC_Settings_Dialog;
}

class LinkC_Settings_Dialog : public QWidget
{
    Q_OBJECT

public:
    explicit LinkC_Settings_Dialog(QWidget *parent = 0);
    ~LinkC_Settings_Dialog();
    void resizeEvent(QResizeEvent *);

private:
};

#endif // LINKC_SETTINGS_DIALOG_H
