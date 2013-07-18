/**********************************************
 *author 周翔
 *E-mail:604487178@qq.com
 *
 *
 *值得改进的地方
 *1.动画的加入
 *2.样式
 *3.会出现闪屏的现象
 ****************************************************/


#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>
#include <QTimeLine>
#include <QLabel>
#include <QLayout>
#include <QEventLoop>


class MessageBox : public QWidget
{
    Q_OBJECT
public:
    static void start(QWidget *parent, const QString &mes = QString(),int sec = 3);
signals:
    void sig_show();
    void sig_close();
public slots:


private slots:
    void slt_desalting(qreal opacity);
protected:
    void closeEvent(QCloseEvent *e);
private:
    int second;//秒
    QString message;
    QTimeLine *timerProducer;
    QLabel *lable;
    explicit MessageBox(QWidget *parent, const QString & mes = QString(),int sec = 3);
    void show();
    void initUI();
};

#endif // MESSAGEBOX_H
