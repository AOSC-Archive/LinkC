#include <string.h>
#include <curses.h>
#include <locale.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "def.h"

#define MAX_ERROR	200

int size = sizeof(struct message_t);
socklen_t addr_len = sizeof(struct sockaddr_in);

struct pthread_data
{
	WINDOW* history;	// The Chating History
	pthread_mutex_t lock;	// Muteex Lock
	int UDP_socket;		// UDP Socket
	struct sockaddr_in peer;// peer's Intelnet Address
};
/* Recv Message And Tell Them From Messsage or HeartBeat,if it is Message , show you it */
void recv_msg(struct pthread_data *data);
/* Send HeartBeat to Keep The NAT HOLE Open and Tell peer You are Still Alive */
void heartbeat(struct pthread_data *data);

int testing_ui(int sockfd,struct sockaddr_in peer)
{
	int i;						// Tmp Argument
	char buffer[size];				// Buffer
	struct message_t message;			// Message Struct for Sending Message
	struct pthread_data data;			// Pthread Data
	char *ch;					// For Getting The Message you Inputteed
	pthread_t recver;				// Pthread ID for recv
	pthread_t beats;				// Pthread IP for HeartBeat
	pthread_mutex_t lock;				// Mutex Lock
	pthread_mutex_init(&lock,NULL);			// Init Lock
	setlocale(LC_ALL,"");				// Set Local
	initscr();					// init Screen
	echo();						// echo on
	WINDOW* history;				// History Window[Show What you Says And What The Peer Says]
	WINDOW* bottom;					// Bottom Window[For input The Message you Want to Send to peer]
	history = subwin(stdscr,LINES-3,COLS,0,0);	// Set Location
	bottom = subwin(stdscr,3,COLS,LINES-3,0);	// Set Location
	scrollok(history,TRUE);				// Set The History Window Scrollable

	data.history = history;				// Save History Window
	data.lock = lock;				// Save Lock
	data.UDP_socket = sockfd;			// Save Sockfd
	data.peer = peer;				// Save Peer's Address

	pthread_create(&recver,NULL,(void *)*recv_msg,&data);		// creat pthread for recv
	pthread_create(&beats,NULL,(void *)*heartbeat,&data);		// creat pthread for send heartbeat
	for(i=0;i<COLS;i++)
		mvwaddch(bottom,0,i,'-');		// Printf "------------"
	while(1)
	{
		memset((void*)&message,'\0',size);	// clear message
		waddstr(bottom,"Input Your Message : ");// printf "Input Your Message : " in Bottom Window
		wgetstr(bottom,ch);			// Get The Data that you inputted in the Bottom Window
		strncpy(message.message,ch,strlen(ch));
		message.check_time = time(0);		// Get Time
		message.header = MESSAGE;		// Set Header is Message
		pthread_mutex_lock(&lock);		// Lock
		for(i=0;i<5;i++)
			if ((sendto(sockfd,(void *)&message,size,MSG_DONTWAIT,(struct sockaddr *)&peer,addr_len)) <= 0)	// Send Message
        		{
				perror("FAILURE");
        		}
		pthread_mutex_unlock(&lock);		// Unlock
		waddstr(history,"Me:\t");		// Printf "Me:\t" in History Window
		waddstr(history,message.message);	// Printf The Message Which you Inputted
		waddch (history,'\n');			// New Line
		werase (bottom);			// Clear Bottom Window
		for(i=0;i<COLS;i++)
			mvwaddch(bottom,0,i,'-');	// Printf "------------"
		wrefresh(history);			// Fresh History Window
		wrefresh(bottom);			// Frish Bottom Window
	}
	refresh();			// Fresh All
	getchar();			// Pause The Program
	endwin();			// End Curses
	pthread_mutex_destroy(&lock);	// Destroy Lock
}

void recv_msg(struct pthread_data *data)
{
	int error=0;			// Save Error Times
	struct message_t message;	// Save The Message we Recved
	time_t check_time;		// Save The Time and tell by the Message that if we recved The Same Message
	while(1)
	{
		if(error > MAX_ERROR)	// If There were so many errors has been coused
		{
			waddstr(data->history,"Peer Has Disconnected!\n");
			break;
		}
		if(recvfrom(data->UDP_socket,(void *)&message,size,MSG_DONTWAIT,(struct sockaddr *)&(data->peer),&addr_len) <= 0)
		{
			error++;
			sleep(1);
			continue;
		}
		if (check_time == message.check_time)	continue;
		switch (message.header)
		{
			case MESSAGE:			// if the message's Header is MESSAGE
			{
				check_time = message.check_time;
				waddstr(data->history,"Peer\t:");
				waddstr(data->history,message.message);	// Can not show Message ,Why?
				waddstr(data->history,"\n");
				break;
			}
			case HEARTBEAT:			// if the message's Header is HEARTBEAT
			{
				break;
			}
		}
		wrefresh(data->history);		// Fresh History Window
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
