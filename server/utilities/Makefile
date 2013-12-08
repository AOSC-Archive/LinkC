CC		= gcc
DEL_FILE	= rm -f
INCLUDE		= -I./../include

####### Output directory

OBJECTS_DIR   = ./

####### Files
SOURCES       = packet_control.c	\

OBJECTS       = packet_control.o	\

first: all

all : $(OBJECTS)


clean:
	-$(DEL_FILE) $(OBJECTS)


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET)

####### Compile

packet_control.o : packet_control.c
	$(CC) -c packet_control.c $(INCLUDE)

