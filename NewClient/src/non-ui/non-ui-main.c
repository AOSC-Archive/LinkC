#include "../../include/linkc_client.h"
#include "../../include/linkc_error.h"
#include "../../include/linkc_client_nonui.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <curses.h>
#include <string.h>


void InitCurses()
{
    initscr();
    cbreak();
    nl();
    noecho();
    start_color();
    intrflush(stdscr,FALSE);
    keypad(stdscr,TRUE);
    init_pair(GREEN_ON_BLACK,COLOR_GREEN,COLOR_BLACK);
    init_pair(RED_ON_BLACK,COLOR_RED,COLOR_BLACK);
    refresh();
}
int GetPassword(char* passwd, int size)
{
   int c;
   int n = 0;
  
   do{
      c = getch();
      if (c != '\n'){
          echochar('*');//printw("*");
         passwd[n++] = c;
        }
   }while(c != '\n' && n < (size - 1));
   passwd[n] = '\0';
   return n;
}

int NonUiMode(){
    InitCurses();                                               //  初始化文本环境
    char Input;
    WINDOW *Console = newwin(10,60,LINES-11,3);                    //  新建窗口
    scrollok(Console,TRUE);                                        //  设置成滚动窗口
    wrefresh(Console);                                             //  刷新窗口
    LinkC_Debug(Console,"初始化LinkC文本界面客户端",LINKC_STARTED);
    sleep(1);                                                   //  暂停1秒

    LinkC_Debug(Console,"初始化LinkC文本界面客户端",LINKC_DONE);
    int Sockfd = InitNetwork();
TRY:
    LinkC_Debug(Console,"连接到服务器",LINKC_STARTED);
    if(ConnectToServer(Sockfd) == LINKC_FAILURE){
        wattron(Console,RED_ON_BLACK);
        wprintw(Console,"连接到服务器失败\n输入R重连，其余键退出\n>>");
        wrefresh(Console);
        Input = getchar();
        if(Input == 'R')    goto TRY;
        return LINKC_FAILURE;
    }
    LinkC_Debug(Console,"连接到服务器",LINKC_SUCCESS);
    LinkC_Debug(Console,"登录",LINKC_STARTED);
    if(NonUiLogin(Sockfd,Console) == LINKC_FAILURE){
        attron(COLOR_PAIR(RED_ON_BLACK));
        LinkC_Debug(Console,"登录",LINKC_FAILURE);
        return LINKC_FAILURE;
    }
    LinkC_Debug(Console,"登录",LINKC_SUCCESS);
    getchar();
    return 0;
}

int NonUiLogin(int Sockfd,WINDOW* Console){
    LoginData Data;
    bzero((void*)&Data,sizeof(LoginData));
    WINDOW *LoginWindow=newwin(10,30,LINES/2-5,COLS/2-15);
    box(LoginWindow,0,0);
    echo();
    wprintw(LoginWindow,"登录");
    wrefresh(LoginWindow);
    mvwaddstr(LoginWindow,3,6,"帐号:");
    wmove(LoginWindow,3,12);
    wrefresh(LoginWindow);
    wgetstr(LoginWindow,Data.UserName);
    noecho();
    mvwaddstr(LoginWindow,4,6,"帐号:");
    wmove(LoginWindow,4,12);
    wrefresh(LoginWindow);
    wgetstr(LoginWindow,Data.PassWord);
    return Login(Sockfd,Data, Console);
}
