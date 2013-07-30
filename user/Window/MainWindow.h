#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "../NetWork/csocket.h"
#include "../include/LinkC_GUI.h"
#include "../include/data_type.h"
#include <qt4/QtGui/QToolBox>
#include <qt4/QtGui/QGroupBox>
#include <qt4/QtGui/QLayout>
#include <qt4/QtGui/QVBoxLayout>
#include <qt4/QtGui/QMainWindow>
#include <qt4/QtGui/QInputDialog>
#include <qt4/QtGui/QToolButton>
#include <qt4/QtGui/QLabel>
#include <qt4/QtGui/QWidget>
#include <qt4/QtGui/QListWidget>
#include <qt4/QtGui/QScrollArea>

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
    void ChatWith(int UID);
signals:
    void test_signal(int i);
private:
    typedef QMap<int,ChatDialog *> _Map;
	csocket server;
	int Connection_state;
    FriendArea * area;
    QTabWidget * tab;
    QGridLayout* TopLayout;
    QVBoxLayout* MainLayout;
    QGridLayout* layout;
    QWidget    * Top;
    QWidget    * MainWidget;
    _Map         ChatDialogMap;
    _Map::iterator tmp;
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


#endif
