CC		= gcc
DEL_FILE	= rm -f
INCLUDE		= -I./../include

####### Output directory

OBJECTS_DIR   = ./

####### Files
SOURCES       = p2p_helper.c \
		conn_list.c \
		list.c
OBJECTS       = p2p_helper.o \
		conn_list.o \
		list.o

first: all

all : $(OBJECTS)


clean:
	-$(DEL_FILE) $(OBJECTS)


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET)

####### Compile

p2p_helper.o : p2p_helper.c	\
		../include/conn_list.h	\
		../include/list.h	\
		conn_list.c	\
			list.c
	$(CC) -c p2p_helper.c $(INCLUDE)
	
conn_list.o : conn_list.c	\
		../include/conn_list.h	\
		../include/list.h	\
		list.c
	$(CC) -c conn_list.c $(INCLUDE)
	
list.o	:	list.c	\
		../include/list.h
	$(CC) -c list.c $(INCLUDE)
	
