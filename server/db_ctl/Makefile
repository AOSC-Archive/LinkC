CC		= gcc
DEL_FILE	= rm -f
INCLUDE		= -I./../include

####### Output directory

OBJECTS_DIR   = ./

####### Files
SOURCES       = linkc_db.c
OBJECTS       = linkc_db.o

first: all

all : $(OBJECTS)


clean:
	-$(DEL_FILE) $(OBJECTS)


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET)

####### Compile

linkc_db.o : linkc_db.c
	$(CC) -c linkc_db.c $(INCLUDE)
