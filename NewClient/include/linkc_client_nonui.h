#ifndef _LINKC_CLIENT_NONUI_
#define _LINKC_CLIENT_NONUI_

#include <curses.h>

int     NonUiLogin(int Sockfd);
/*  在文本模式下登录        */
WINDOW *CommandLine;
WINDOW *Console;

void     GetCommandLine(char *Buffer);


#define GREEN_ON_BLACK  1
#define RED_ON_BLACK    2

#endif
