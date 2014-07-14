#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "loginwindow.h"
#include "linkc_client.h"
#include "linkc_socket.h"
#include "linkc_def.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void        LoginStatus(bool);


public slots:
    void        ExitProgram(void);
    void        DoLogin(LoginData);

private:
    Ui::MainWindow *ui;
    LoginWindow *LoginW;
    TCP_Socket  *Socket;
    void        *Buffer;
    void        *Package;
};

#endif // MAINWINDOW_H
