MAKE		= make
QMAKE		= qmake-qt4

####### Output directory

OBJECTS_DIR   = ./

####### Files
TARGET		= LINKC
first: all

all : $(TARGET)

$(SERVER) : server/Makefile

$(CLIENT) : user/LinkC_Client.pro

clean:
	$(MAKE) clean -C user && $(MAKE) clean -C server

clean_all:
	$(MAKE) clean -C user && $(MAKE) clean -C server && rm user/LinkC_Client user/LinkC_Client.pro.user user/Makefile server/LinkC_Server

####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET)

####### Compile
$(TARGET) : $(SERVER) $(CLIENT)
	$(MAKE) -C server
	cd  user && $(QMAKE)
	$(MAKE) -C user
