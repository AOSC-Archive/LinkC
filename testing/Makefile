CC		= gcc
DEL_FILE	= rm -f
LIBS		= -lncursesw -lpthread

####### Output directory

OBJECTS_DIR   = ./

####### Files
SOURCES       = peer_client_test.c	\
		testing_ui.c
HEADERS	      = conn_list.h  \
		def.h  \
		list.h
OBJECTS       = peer_client_test.o	\
		testing_ui.o
TARGET	      = peer_client_test

first: all

all : $(TARGET)

tar : $(SOURCES)
	tar -jcvf p2p_client_test.tar.gz $(SOURCES) $(HEADERS) Makefile

clean:
	-$(DEL_FILE) $(OBJECTS)


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET)

####### Compile

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBS)
peer_client_test.o : peer_client_test.c
	$(CC) -c peer_client_test.c $(INCLUDE)
testing_ui.o : testing_ui.c
	$(CC) -c testing_ui.c $(INCLUDE)

