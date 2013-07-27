#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt4/QtGui/QMainWindow>
#include <qt4/QtGui/QInputDialog>
#include <qt4/QtNetwork/QTcpSocket>
#include "../NetWork/csocket.h"
#include "../include/Friend.h"
#include <qt4/Qt/qtoolbox.h>
#include <qt4/Qt/qgroupbox.h>
#include <qt4/Qt/qlayout.h>
#include <qt4/QtGui/QVBoxLayout>
#include <qt4/Qt/qtoolbutton.h>
#include <QLabel>
#include <QWidget>
#include "../include/data_type.h"
#include <QListWidget>
#include <QScrollArea>

namespace Ui {
class MainWindow;
class LoginWindow;
class ChatDialog;
}

class MainWindow : public QMainWindow{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);	   //构造函数
	~MainWindow();	  // 析构函数
	int Login();
	int NetworkInit(void);
    void resizeEvent(QResizeEvent *);
    int InitFriendList();
    FriendArea *friendArea;
private slots:
    void check(void);
    void test_slot(int i);
signals:
    void test_signal(int i);
private:
	csocket server;
	int Connection_state;
    FriendArea * area;
    QTabWidget * tab;
    QGridLayout* TopLayout;
    QVBoxLayout* MainLayout;
    QGridLayout* layout;
    QWidget    * Top;
    QWidget    * MainWidget;
};








class LoginWindow : public QDialog{
	Q_OBJECT

public:
    explicit LoginWindow(QWidget* = 0);	   //构造函数
	~LoginWindow();	  // 析构函数
	int GetLoginData(login_data &s);
	QPushButton* LoginButton;
	QPushButton* CancelButton;
public slots:		   // 槽函数
	void QuitClick(void);		 // 退出按钮被按下
	void LoginClick(void);		// 登录按钮被按下
private:
	const char* UserName;
	const char* PassWord;
    QLineEdit * UsernameInput;
    QLineEdit * PasswordInput;


	struct login_data st;
};


/*class ChatDialog : public QDialog{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = 0);
    ~ChatDialog();
public slots:
    void chatwith(const friend_data data);
};
*/
#endif
