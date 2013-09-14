CC		= gcc
DEL_FILE	= rm -f
INCLUDE		= -I./../include

####### Output directory

OBJECTS_DIR   = ./

####### Files
SOURCES       = chat_with.c \
		user_login.c \
		user_logout.c

OBJECTS       = chat_with.o \
		user_login.o \
		user_logout.o

first: all

all : $(OBJECTS)


clean:
	-$(DEL_FILE) $(OBJECTS)


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET)

####### Compile

chat_with.o :   chat_with.c \
		../include/linkc_types.h \
		../include/linkc_db.h \
		../include/linkc_user.h
	$(CC) -c chat_with.c $(INCLUDE)

user_login.o :  user_login.c \
		../include/linkc_db.h \
		../include/linkc_types.h \
		../include/linkc_user.h
	$(CC) -c user_login.c $(INCLUDE)

user_logout.o : user_logout.c \
		../include/linkc_db.h \
		../include/linkc_user.h
	$(CC) -c user_logout.c $(INCLUDE)
