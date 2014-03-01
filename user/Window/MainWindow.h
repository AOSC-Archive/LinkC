#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "Csocket.h"
#include "LinkC_GUI.h"
#include "linkc_types.h"
#include "MessageRecver.h"
#include "Utilities/LinkC_Utilities.h"
#include <QToolBox>
#include <QGroupBox>
#include <QLayout>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QInputDialog>
#include <QToolButton>
#include <QLabel>
#include <QWidget>
#include <QListWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QListView>
#include <QMessageBox>
#include <QLineEdit>
#include <QDialog>
#include <QString>

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
    void closeEvent(QCloseEvent *);
    int InitFriendList();
    FriendArea *friendArea;
    void *package;
private slots:
    void ChatWith(LinkC_Friend_Data);
    void FriendLabelClicked(LinkC_Friend_Data);
    void UserMessage(LinkC_User_Message);
    void SysActionStatus(LinkC_Sys_Status);
    void SendMessageToServer(LinkC_User_Request);
signals:
private:
    typedef QMap<int,ChatDialog *> _Map;
    TCP_csocket server;
	int Connection_state;
    FriendArea * area;
    QTabWidget * tab;
    QGridLayout* TopLayout;
    QVBoxLayout* MainLayout;
    QGridLayout* layout;
    QWidget    * Top;
    QWidget    * MainWidget;
    _Map         ChatDialogMap;
    _Map::iterator ChatDialogiterator;

    TCP_MessageRecver *Recver;
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
