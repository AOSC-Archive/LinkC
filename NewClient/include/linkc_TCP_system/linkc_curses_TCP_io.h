#ifndef LINKC_CURSES_TCP_IO_H
#define LINKC_CURSES_TCP_IO_H
#include <curses.h>
#include <stdint.h>
int16_t wTCP_Recv(WINDOW* Console,int Sockfd, void *Out, int Out_size, int flag);

#endif
