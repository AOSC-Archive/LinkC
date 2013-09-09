CC		= gcc
DEL_FILE	= rm -f

####### Output directory

OBJECTS_DIR   = ./

####### Files
SOURCES       = peer_client_test.c
HEADERS	      = conn_list.h  \
		def.h  \
		list.h
OBJECTS       = peer_client_test.o
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

$(TARGET) : $(OBJECTS) $(SOURCES)
	$(CC) -o $(TARGET) $(OBJECTS)
peer_client_test.o : peer_client_test.c
	$(CC) -c peer_client_test.c $(INCLUDE)

