/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QPushButton *SiginButton;
    QPushButton *QuitButton;
    QLineEdit *UsernameEditor;
    QLineEdit *PasswordEditor;
    QLabel *SigupLabel;
    QLabel *UsernameLabel;
    QLabel *PasswordLabel;

    void setupUi(QWidget *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName(QStringLiteral("LoginWindow"));
        LoginWindow->resize(240, 150);
        LoginWindow->setMinimumSize(QSize(240, 150));
        LoginWindow->setMaximumSize(QSize(240, 150));
        LoginWindow->setLocale(QLocale(QLocale::C, QLocale::AnyCountry));
        SiginButton = new QPushButton(LoginWindow);
        SiginButton->setObjectName(QStringLiteral("SiginButton"));
        SiginButton->setGeometry(QRect(20, 90, 80, 22));
        QuitButton = new QPushButton(LoginWindow);
        QuitButton->setObjectName(QStringLiteral("QuitButton"));
        QuitButton->setGeometry(QRect(130, 90, 80, 22));
        UsernameEditor = new QLineEdit(LoginWindow);
        UsernameEditor->setObjectName(QStringLiteral("UsernameEditor"));
        UsernameEditor->setGeometry(QRect(90, 20, 113, 22));
        UsernameEditor->setMaxLength(16);
        PasswordEditor = new QLineEdit(LoginWindow);
        PasswordEditor->setObjectName(QStringLiteral("PasswordEditor"));
        PasswordEditor->setGeometry(QRect(90, 50, 113, 22));
        PasswordEditor->setMaxLength(16);
        PasswordEditor->setEchoMode(QLineEdit::Password);
        SigupLabel = new QLabel(LoginWindow);
        SigupLabel->setObjectName(QStringLiteral("SigupLabel"));
        SigupLabel->setGeometry(QRect(140, 120, 71, 20));
        SigupLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        UsernameLabel = new QLabel(LoginWindow);
        UsernameLabel->setObjectName(QStringLiteral("UsernameLabel"));
        UsernameLabel->setGeometry(QRect(10, 20, 71, 21));
        UsernameLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        PasswordLabel = new QLabel(LoginWindow);
        PasswordLabel->setObjectName(QStringLiteral("PasswordLabel"));
        PasswordLabel->setGeometry(QRect(20, 50, 61, 20));
        PasswordLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QWidget *LoginWindow)
    {
        LoginWindow->setWindowTitle(QApplication::translate("LoginWindow", "Login to LinkC", 0));
        SiginButton->setText(QApplication::translate("LoginWindow", "Login", 0));
        QuitButton->setText(QApplication::translate("LoginWindow", "Exit", 0));
        SigupLabel->setText(QApplication::translate("LoginWindow", "Sign Up", 0));
        UsernameLabel->setText(QApplication::translate("LoginWindow", "Account", 0));
        PasswordLabel->setText(QApplication::translate("LoginWindow", "Password", 0));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
