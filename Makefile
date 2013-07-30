MAKE		= make

####### Output directory

OBJECTS_DIR   = ./

####### Files
TARGET		= LINKC
first: all

all : $(TARGET)

$(SERVE) : server/Makefile

$(CLIENT) : user/Makefile

clean:
	$(MAKE) clean -C user && $(MAKE) clean -C server

####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET)

####### Compile
$(TARGET) : $(SERVE) $(CLIENT)
	$(MAKE) -C server
	qmake-qt4 LinkC_Cilent.pro
	$(MAKE) -C user
