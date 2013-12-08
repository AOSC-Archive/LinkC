CC		= gcc
CXX		= g++
LIBS		= -lsqlite3 -lpthread
DEL_FILE	= rm -f
INCLUDE		= -I./include
SYMLINK		= ln -f -s
DEL_DIR		= rmdir
MOVE		= mv -f
CHK_DIR_EXISTS	= test -d
MKDIR		= mkdir -p
MAKE		= make	

####### Output directory

OBJECTS_DIR   = ./

####### Files
DEPEND	      = /usr/include/sqlite3.h
SOURCES       = main.c			\
		network/keep_connect.c 	\
		network/start_connect.c \
		network/init_network.c 	\
		list/conn_list.c	\
		list/list.c		\
		p2p/p2p_helper.c	\
		user/user_logout.c 	\
		user/user_login.c 	\
		user/send_friends_data.c\
		db_ctl/linkc_db.c	\
		utilities/packet_control.c

NETWORK       = network/keep_connect.o	\
		network/start_connect.o	\
		network/init_network.o

USER_CTL     =  user/user_logout.o	\
		user/user_login.o	\
		user/send_friends_data.o
		
P2P	      = p2p/p2p_helper.o	\
		list/conn_list.o	\
		list/list.o

DB_CTL	      = db_ctl/linkc_db.o

UTILITIES     = utilities/packet_control.o

OBJECT       = main.o

TARGET	      = LinkC_Server

first: Debug

new : $(TARGET)

Debug : $(TARGET)

$(USER_CTL) : user/Makefile
	$(MAKE) -C user

$(NETWORK)	: network/Makefile
	$(MAKE) -C network

$(DB_CTL) : db_ctl/Makefile
	$(MAKE) -C db_ctl
	
$(P2P)	: p2p/Makefile
	$(MAKE) -C p2p
	$(MAKE) -C list

$(UTILITIES)	:	utilities/Makefile
	$(MAKE) -C utilities

clean:
	$(MAKE) clean -C network && $(MAKE) clean -C user && $(MAKE) clean -C utilities && $(MAKE) clean -C p2p && $(MAKE) clean -C list && $(MAKE) clean -C db_ctl && $(DEL_FILE) $(OBJECT)

cleanDebug:
	$(MAKE) clean -C network && $(MAKE) clean -C user && $(MAKE) clean -C db_ctl && $(MAKE) clean -C p2p && $(DEL_FILE) $(OBJECT) $(TARGET)

rebuild:
	$(MAKE) clean && $(MAKE)

####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET)

####### Compile

main.o: main.c
	$(CC) -c main.c $(INCLUDE)

$(TARGET): $(OBJECT) $(NETWORK) $(USER_CTL) $(DB_CTL) $(P2P) $(SOURCES) $(P2P) $(UTILITIES) $(DEPEND)
	$(CC) -o $(TARGET) $(OBJECT) $(NETWORK) $(USER_CTL) $(P2P) $(DB_CTL) $(UTILITIES) $(LIBS) $(INCLUDE)

