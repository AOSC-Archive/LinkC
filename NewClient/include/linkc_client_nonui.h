#ifndef _LINKC_CLIENT_NONUI_
#define _LINKC_CLIENT_NONUI_

#include <curses.h>
#include "linkc_def.h"

int NonUiMode();
/*  进入NonUi模式   */
int     NonUiLogin(WINDOW* Console,int Sockfd);
/*  在文本模式下登录        */
int wLogin(WINDOW* Console,int Sockfd,LoginData Data);
/*  登录            */

int wGetSelfData(WINDOW* Console, int Sockfd,UserData *Data);

void GetCommandLine(char *Buffer);

int wGetFriendsData(WINDOW* Console,int Sockfd,UserData* Data);



#define GREEN_ON_BLACK  1
#define RED_ON_BLACK    2

#endif
