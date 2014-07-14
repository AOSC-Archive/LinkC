#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "linkc_def.h"
#include "linkc_client.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

signals:
    void SIGN_LoginStart(LoginData);
    void SIGN_QuitButtonClicked(void);

private slots:
    void SLOT_LoginButtonClicked(void);
    void SLOT_QuitButtonClicked(void);

private:
    Ui::LoginWindow *ui;
    LoginData Data;
};

#endif // LOGINWINDOW_H
