CC		= gcc
CXX		= g++
LIBS		= -lsqlite3 -lpthread
DEL_FILE	= rm -f
SYMLINK		= ln -f -s
DEL_DIR		= rmdir
MOVE		= mv -f
CHK_DIR_EXISTS	= test -d
MKDIR		= mkdir -p
MAKE		= make	

####### Output directory

OBJECTS_DIR   = ./

####### Files
SOURCES       = main.c \
		network/keep_connect.c \
		network/start_connect.c \
		network/init_network.c \
		user/chat_with.c \
		user/user_logout.c \
		user/user_login.c \
		db_ctl/linkc_db.c

NETWORK	      = network/keep_connect.o \
		network/start_connect.o \
		network/init_network.o

USER_CTL      = user/chat_with.o \
		user/user_logout.o \
		user/user_login.o

DB_CTL	      = db_ctl/linkc_db.o

OBJECT       = main.o

TARGET	      = LinkC_Server

first: all

all : $(TARGET)

$(USER_CTL) : user/Makefile
	$(MAKE) -C user

$(NETWORK) : network/Makefile
	$(MAKE) -C network

$(DB_CTL) : db_ctl/Makefile
	$(MAKE) -C db_ctl

clean:
	$(MAKE) clean -C network && $(MAKE) clean -C user && $(MAKE) clean -C db_ctl && $(DEL_FILE) $(OBJECT)

clean_all:
	$(MAKE) clean -C network && $(MAKE) clean -C user && $(MAKE) clean -C db_ctl && $(DEL_FILE) $(OBJECT) $(TARGET)

rebuild:
	$(MAKE) clean && $(MAKE)

####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET)

####### Compile

main.o: main.c
	$(CC) -c main.c

$(TARGET): $(OBJECT) $(NETWORK) $(USER_CTL) $(DB_CTL)
	$(CC) -o $(TARGET) $(OBJECT) $(NETWORK) $(USER_CTL) $(DB_CTL)  $(LIBS)
