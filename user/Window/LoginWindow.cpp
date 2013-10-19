#include "MainWindow.h"
#include "../include/data_type.h"
#include <unistd.h>
#include <string.h>
#include <qt4/QtGui/QtGui>
#include <qt4/QtGui/QMessageBox>
#include <qt4/QtCore/QString>
#include <qt4/QtGui/QLineEdit>
#include <qt4/QtGui/QDialog>





#define _LINIC_     1

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent){
    QLabel* usrLabel = new QLabel(tr("UserName:"));
    QLabel* pwdLabel = new QLabel(tr("PassWord:"));
    UsernameInput = new QLineEdit;
    PasswordInput = new QLineEdit;
    PasswordInput->setEchoMode(QLineEdit::Password);
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(usrLabel, 0, 0, 1, 1);
    gridLayout->addWidget(UsernameInput, 0, 1, 1, 3);
    gridLayout->addWidget(pwdLabel, 1, 0, 1, 1);
    gridLayout->addWidget(PasswordInput, 1, 1, 1, 3);
    LoginButton = new QPushButton(tr("Login"));
    CancelButton = new QPushButton(tr("Cancel"));
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(50);
    btnLayout->addWidget(LoginButton);
    btnLayout->addWidget(CancelButton);
    QVBoxLayout* dlgLayout = new QVBoxLayout;
    dlgLayout->setMargin(40);
    dlgLayout->addLayout(gridLayout);
    dlgLayout->addStretch(40);
    dlgLayout->addLayout(btnLayout);
    setLayout(dlgLayout);
    connect(LoginButton, SIGNAL(clicked()), this, SLOT(LoginClick()));
    connect(CancelButton, SIGNAL(clicked()), this, SLOT(QuitClick()));
    setWindowTitle(tr("Login"));
    resize(300, 200);
    setMinimumSize(300,200);
    setMaximumSize(300,200);
}

LoginWindow::~LoginWindow(){
    printf ("Debug >> Login_Window Exited Normally!\n");
}

void LoginWindow::QuitClick(void){
    QString response = "Are you sure to exit?";
    int answer = 0;
    answer = QMessageBox::question(0,"Question",response,QMessageBox::Yes|QMessageBox::No);
    if (answer == QMessageBox::Yes){
        exit(0);
    }
}

void LoginWindow::LoginClick(void){

    accept();
}

int LoginWindow::GetLoginData(struct login_data& s){
    QString aa = UsernameInput->text();
    QString ab = PasswordInput->text();
    if (aa == ""){
        QMessageBox::warning(0,"Warning","The Username is empty!",QMessageBox::Yes);
        return -1;
    }
    else if (ab == ""){
        QMessageBox::warning(0,"Warning","The Password is enpty!",QMessageBox::Yes);
        return -1;
    }
    QByteArray ba = aa.toLatin1();
    UserName = ba.data();
    strncpy (st.user_name,ba.data(),12);
    ba = ab.toLatin1();
    PassWord = ba.data();
    strncpy (st.pass_word,ba.data(),20);
    s = st;
    return 0;
}
