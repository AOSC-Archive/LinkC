#include <string.h>
#include <curses.h>
#include <locale.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "def.h"

#define MAX_ERROR	5

int size = sizeof(struct message_t);
socklen_t addr_len = sizeof(struct sockaddr_in);

struct pthread_data
{
	WINDOW* history;	// 历史信息框
	pthread_mutex_t lock;	// 互斥锁
	int UDP_socket;		// UDP连接句柄
	struct sockaddr_in peer;// peer's Intelnet Address
};
void recv_msg(struct pthread_data *data);
void heartbeat(struct pthread_data *data);

int testing_ui(int sockfd,struct sockaddr_in peer)
{
	setlocale(LC_ALL,"");
	pthread_mutex_t lock;
	pthread_mutex_init(&lock,NULL);
	int i;
	char buffer[size];
	struct message_t message;
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
	pthread_t beats;
	pthread_create(&recver,NULL,(void *)*recv_msg,&data);		// creat pthread for recv
	pthread_create(&beats,NULL,(void *)*heartbeat,&data);	// creat pthread for send heartbeat
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
			if ((sendto(sockfd,(void *)&message,size,MSG_DONTWAIT,(struct sockaddr *)&peer,addr_len)) < 0)
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
	int error=0;
	struct message_t message;
	time_t check_time;
	char *time_to_print=NULL;
	while(1)
	{
		if(error > MAX_ERROR)
		{
			waddstr(data->history,"Peer Has Disconnected!\n");
			break;
		}
		if(recvfrom(data->UDP_socket,(void *)&message,size,0,(struct sockaddr *)&(data->peer),&addr_len) <= 0)
		{
			error++;
			continue;
		}
		if (check_time == message.check_time)	continue;
		switch (message.header)
		{
			case MESSAGE:			// if the message is MESSAGE
			{
				check_time = message.check_time;
				waddstr(data->history,"Peer[");
				waddstr(data->history,ctime(&check_time));
				waddstr(data->history,"] : ");
				waddstr(data->history,message.message);
				waddstr(data->history,"\n");
				break;
			}
			case HEARTBEAT:			// if the message only a heartbeat
			{
				break;
			}
		}
		error = 0;
	}
}
void heartbeat(struct pthread_data *data)
{
	struct message_t message;
	message.header = HEARTBEAT;
	int i;
	while(1)
	{
		pthread_mutex_lock(&(data->lock));		// Locked
		for(i=0;i<5;i++)
			if ((sendto(data->UDP_socket,(void *)&message,size,MSG_DONTWAIT,(struct sockaddr *)&(data->peer),addr_len)) < 0)
	        	{
				perror("FAILURE");
	        	}
		pthread_mutex_unlock(&(data->lock));		// Unlock
		sleep(5);
	}
}
