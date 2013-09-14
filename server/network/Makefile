CC		= gcc
DEL_FILE	= rm -f
INCLUDE		= -I./../include/

####### Output directory

OBJECTS_DIR   = ./

####### Files
SOURCES       = start_connect.c \
		keep_connect.c \
		init_network.c

OBJECTS       = start_connect.o \
		keep_connect.o \
		init_network.o

first: all

all : $(OBJECTS)


clean:
	-$(DEL_FILE) $(OBJECTS)


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET)

####### Compile

start_connect.o:start_connect.c \
		../include/linkc_db.h \
		../include/linkc_types.h \
		../include/linkc_network.h
	$(CC) -c start_connect.c $(INCLUDE)

keep_connect.o : keep_connect.c \
		../include/linkc_network.h \
		../include/linkc_types.h \
		../include/linkc_user.h \
		../include/linkc_db.h
	$(CC) -c keep_connect.c $(INCLUDE)

init_network.o: init_network.c \
		../include/linkc_network.h
	$(CC) -c init_network.c $(INCLUDE)
