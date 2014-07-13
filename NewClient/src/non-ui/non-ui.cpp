#include "../../include/linkc_def.h"
#include "../../include/linkc_client.h"
#include "../../include/linkc_curses_error.h"
#include "../../include/linkc_client_nonui.h"
#include "../../include/linkc_package.h"
#include "../../include/linkc_package_ctl.h"
#include "../../include/linkc_basic_network.h"
#include "../../include/linkc_TCP_system/linkc_TCP_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <curses.h>
#include <string.h>

WINDOW* CommandLine;

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
    WINDOW* Console = newwin(10,63,LINES-13,3);                    //  新建窗口
    CommandLine = newwin(2,63,LINES-3,3);
    scrollok(Console,TRUE);                                        //  设置成滚动窗口
    scrollok(CommandLine,TRUE);                                        //  设置成滚动窗口
    wrefresh(Console);                                             //  刷新窗口
    wrefresh(CommandLine);
    wLinkC_Debug(Console,"初始化LinkC文本界面客户端",LINKC_STARTED);
    char Command[STD_PACKAGE_SIZE];
    void *Package = malloc(STD_PACKAGE_SIZE);
    void *Buffer  = malloc(STD_PACKAGE_SIZE);
    int   Length  = 0;
    bzero(Package,STD_PACKAGE_SIZE);
    bzero(Buffer, STD_PACKAGE_SIZE);
    bzero(Command,STD_PACKAGE_SIZE);

    UserData MySelf;

    wLinkC_Debug(Console, "初始化LinkC文本界面客户端",LINKC_DONE);
    int Sockfd = InitNetwork();
TRY:
    wLinkC_Debug(Console, "连接到服务器",LINKC_STARTED);
    if(ConnectToServer(Sockfd) == LINKC_FAILURE){
        wattron(Console,RED_ON_BLACK);
        wprintw(Console,"连接到服务器失败\n输入R重连，其余键退出\n");
        wrefresh(Console);
        touchwin(CommandLine);
        Input = getch();
        if(Input == 'R')    goto TRY;
        return LINKC_FAILURE;
    }
    wLinkC_Debug(Console,"连接到服务器",LINKC_SUCCESS);
    wLinkC_Debug(Console,"登录",LINKC_STARTED);
    if(NonUiLogin(Console,Sockfd) == LINKC_FAILURE){
        attron(COLOR_PAIR(RED_ON_BLACK));
        wLinkC_Debug(Console,"登录",LINKC_FAILURE);
        return LINKC_FAILURE;
    }
    wLinkC_Debug(Console,"登录",LINKC_SUCCESS);
    while(1){
        GetCommandLine(Command);
        if(strcmp(Command,"Logout")==0){
            ((MessageHeader*)Buffer)->ActionType    = USER_LOGOUT;
            Length = _Package(Buffer,sizeof(MessageHeader),NORMAL_MESSAGE,Package);
            TCP_Send(Sockfd,Package,Length,0);
            wLinkC_Debug(Console,"等待服务端的回应......",LINKC_DEBUG);
            wTCP_Recv(Console,Sockfd,Buffer,STD_PACKAGE_SIZE,0);
            _UnPackage(Buffer,STD_PACKAGE_SIZE,Package);
            if(ntohs(((MessageHeader*)Package)->StatusCode) == LOGOUT_SUCCESS){
                wLinkC_Debug(Console,"登出",LINKC_SUCCESS);
                wLinkC_Debug(Console,"按任意键退出......",LINKC_DEBUG);
                wrefresh(Console);
                getchar();
                break;
            }else{
                wMemoryPrint(Console,Package,sizeof(MessageHeader));
                wprintw(Console,"退出失败\n");
                wrefresh(Console);
            }
        }else if(strcmp(Command,"GetSelfData")==0){
            wLinkC_Debug(Console,"获取自身资料",LINKC_STARTED);
            if(wGetSelfData(Console,Sockfd,&MySelf) == LINKC_FAILURE){
                wLinkC_Debug(Console,"获取自身资料",LINKC_FAILURE);
                continue;
            }
            wLinkC_Debug(Console,"获取自身资料",LINKC_DONE);
        }else{
            wLinkC_Debug(Console,"没有对应操作",LINKC_WARNING);
            continue;
        }
    }
    free(Package);
    free(Buffer);
    return 0;
}

int NonUiLogin(WINDOW* Console,int Sockfd){
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
    echo();
    wclear(LoginWindow);
    delwin(LoginWindow);
    wclear(stdscr);
    refresh();
    wrefresh(Console);
    return wLogin(Console,Sockfd,Data);
}

void GetCommandLine(char*Buffer){
    echo();
    wprintw(CommandLine,">>");
    wrefresh(CommandLine);
    wgetstr(CommandLine,Buffer);
    return;
}
