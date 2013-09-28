#include <curses.h>
#include <string.h>
#include <locale.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "def.h"


struct pthread_data
{
	WINDOW* history;	// 历史信息框
	pthread_mutex_t lock;	// 互斥锁
	int UDP_socket;		// UDP连接句柄
	struct sockaddr_in peer;// peer's Intelnet Address
};
void recv_msg(struct pthread_data *data);

int testing_ui(int sockfd,struct sockaddr_in peer)
{
	setlocale(LC_ALL,"");
	pthread_mutex_t lock;
	pthread_mutex_init(&lock,NULL);
	int i;
	int size = sizeof(struct message_t);
	char buffer[size];
	struct message_t message;
	socklen_t addr_len = sizeof(struct sockaddr_in);
	initscr();
	echo();
	WINDOW* history;
	history = subwin(stdscr,LINES-3,COLS,0,0);
	WINDOW* bottom;
	bottom = subwin(stdscr,3,COLS,LINES-3,0);
	scrollok(history,TRUE);
	struct pthread_data data;
	data.history = history;
	data.lock = lock;
	data.UDP_socket = sockfd;
	data.peer = peer;
	pthread_t recver;
	pthread_create(&recver,NULL,(void *)*recv_msg,&data);	// creat pthread
	for(i=0;i<COLS;i++)
		mvwaddch(bottom,0,i,'-');
	while(1)
	{
		memset((void*)&message,'\0',size);
		waddstr(bottom,"Input Your Message : ");
		wgetstr(bottom,message.message);
		message.check_time = time(0);
		message.header = MESSAGE;
		pthread_mutex_lock(&lock);		// Locked	
		for(i=0;i<5;i++)
			if ((sendto(sockfd,(void *)&message,size,0,(struct sockaddr *)&peer,addr_len)) < 0)
        		{
				perror("FAILURE");
        		}
		pthread_mutex_unlock(&lock);		// Unlock
		waddstr(history,"Me:\t");
		waddstr(history,message.message);
		waddch (history,'\n');
		werase (bottom);
		for(i=0;i<COLS;i++)
			mvwaddch(bottom,0,i,'-');
		wrefresh(history);
		wrefresh(bottom);
	}
	refresh();
	getchar();
	endwin();
	pthread_mutex_destroy(&lock);
}

void recv_msg(struct pthread_data *data)
{
}
