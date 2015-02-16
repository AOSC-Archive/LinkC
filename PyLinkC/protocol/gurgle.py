#!/usr/bin/python3
#!encoding='utf-8'

import socket
import json
import _thread
import random
import sys
from codecs import decode, encode
import string
import mysql.connector as mysql

class packageNode:
    nextNode    = None
    data        = None
    packageID   = 0
    def __init__(self,nodeData):
        self.data = nodeData

class packageList:
    def __init__(self,newRoot):
        self.size = 0
        self.root = packageNode()
        if newRoot is None:
            self.root = None
            return
        else:
            self.root = newRoot
            self.size += 1
            return
    def __init__(self):
        self.root = None
        self.size = 0
    def __del__(self):
        if self.root is None:
            return
        curNode = self.root
        while curNode.nextNode is not None:
            tempNode = curNode
            curNode = curNode.nextNode
            tempNode = None
        curNode = None
    def insert(self,newData,packageID):
        newNode = packageNode(newData)
        newNode.packageID = packageID
        if self.root is None:
            self.root = newNode
            self.size += 1
            return
        tempNode = self.root
        while tempNode.nextNode is not None:
            tempNode = tempNode.nextNode
        tempNode.nextNode = newNode
        self.size += 1
    def getData(self,packageID):
        if self.size == 0:
            return None
        else:
            tempNode = self.root
            for i in range(0,self.size):
                if tempNode.packageID == packageID:
                    return tempNode.data
                else:
                    tempNode = tempNode.nextNode
            return None
    def remove(self,packageID):
        if self.root is None:
            return
        if self.root.packageID == packageID:
            tempNode = self.root.nextNode
            self.root = None
            self.size -= 1
            self.root = tempNode
            return
        curNode = self.root
        while curNode.nextNode is not None:
            if curNode.nextNode.packageID == packageID:
                tempNode = curNode.nextNode
                curNode.nextNode = curNode.nextNode.nextNode
                tempNode = None  #remove the node,but curNode stays still
                self.size -= 1
            else:
                curNode = curNode.nextNode
    def getRoot(self):
        return self.root
    def getSize(self):
        return self.size
    def printList(self):
        tempNode = self.root
        while tempNode is not None:
            print (tempNode.data)
            tempNode = tempNode.nextNode

class gurgle:
    auth_method_supported               = ["plain"]
    GURGLE_CLIENT                       = 1
    GURGLE_SERVER                       = 2
    GURGLE_GROUP                        = 3
    GURGLE_FAILED                       = False
    GURGLE_SUCCESS                      = True
    GURGLE_HOST_NOT_SET                 = 10
    GURGLE_PORT_NOT_SET                 = 11
    GURGLE_ALREADY_CONNECTED            = 12
    GURGLE_HAVENT_CONNECTED             = 13
    GURGLE_FAILED_TO_AUTH               = 14
    GURGLE_FAILED_TO_CREATE_SOCKET      = 15
    GURGLE_FAILED_TO_CONNECT_TO_REMOTE  = 16
    GURGLE_FAILED_TO_RECV               = 17
    GURGLE_FAILED_TO_SEND               = 18
    GURGLE_CONNECTION_CLOSED            = 19
    def __init__(self,_mode):
        self.__gurgleVersion    = 'Unusable'
        self.__remoteHost       = None
        self.__remotePort       = None
        self.__is_connected     = False
        self.__runtime_mode     = _mode
        self.__socket           = None
        self.__packageList      = packageList();
        self.__auth_method      = 'plain'
        self.__terminal_id      = None
        self.__mysql_fd         = None
        self.__mysql_conn       = None
        if self.__runtime_mode == gurgle.GURGLE_CLIENT:
            print ('Gurgle version',self.__gurgleVersion,'initlalized as Client')
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            print ('Gurgle version',self.__gurgleVersion,'initlalized as Server')
        if self.__runtime_mode == gurgle.GURGLE_GROUP:
            print ('Gurgle version',self.__gurgleVersion,'initlalized as Group server')
        print ('Use authenticated method [%s] by default' %self.get_auth_method())
    def __del__(self):
        print ('Gurgle Deleting....')
    def get_version(self):
        return self.__gurgleVersion;
    def set_socket(self,sockfd):
        self.__socket = sockfd
        self.__is_connected = True
    def create_id(self):
        return random.randint(0,2147483647);
    def create_terminal_id(self):
        if not self.__terminal_id:
            return ''.join(random.sample(string.ascii_letters + string.digits,8))
        else :
            return self.__terminal_id
    def mysql_excute(self,database='linkc_users',cmd=None):
        if not cmd:
            sys.stderr.write('No command will be excuted!\n')
            return None
        try:
            self.__mysql_conn   = mysql.connect(host='localhost',user='root',passwd='root',port=3306)
        except mysql.Error as e:
            sys.stderr.write('Error connecting to SQL server:%s\n'%e)
            return None
        self.__mysql_fd     = self.__mysql_conn.cursor()
        try:
            self.__mysql_fd.select_db(database)
        except mysql.Error as e:
            sys.stderr.write('Error selecting database to [%s]:%s'%(database,e))
            return None
    def recv(self,bufsize):
        try:
            buf = self.__socket.recv(bufsize)
        except socket.error as e:
            sys.stderr.write('Error receiving data: %s\n' % e)
            return None
        if not len(buf):
            sys.stderr.write('Connection closed\n')
            return None
        return buf
    def send(self,buf):
        try:
            self.__socket.send(buf)
        except socket.error as e:
            sys.stderr.write('Error sending data:%s\n' %e)
            return gurgle.GURGLE_FAILED_TO_SEND
        return gurgle.GURGLE_SUCCESS
    def set_remote_host(self,strHost):
        self.__remoteHost = strHost
    def set_remote_port(self,nPort):
        self.__remotePort = nPort
    def get_runtime_mode(self):
        return self.__runtime_mode
    def get_remote_host(self):
        return self.__remoteHost
    def get_remote_port(self):
        return self.__remotePort
    def get_auth_method(self):
        return self.__auth_method;
    def is_remote_addr_set(self):
        if not self.__remoteHost:
            sys.stderr.write('Remote addr is not set!\n')
            return gurgle.GURGLE_HOST_NOT_SET
        if not self.__remotePort:
            sys.stderr.write('Remote port is not set!\n')
            return gurgle.GURGLE_PORT_NOT_SET
        return gurgle.GURGLE_SUCCESS
    def do_auth(self,username, password):
        if not (username and password):
            sys.stderr.write('Username or password is incorrect!\n')
            return gurgle.GURGLE_FAILED_TO_AUTH
        senddata = json.dumps('{"id":"%d", "cmd":"auth", "from":"%s","params":{"method":"%s","password":"%s"}}'
                %(self.create_id(),
                    "grgl:%s@%s/%s"%(username,self.get_remote_host(),self.create_terminal_id()),
                    self.get_auth_method(),password))
        self.send(encode(senddata))
    def ping(self):
        senddata = json.dumps('{"id":"%d", "cmd":"ping"}'
                %self.create_id())
        if self.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024)
        if recvdata is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        data = json.loads(json.loads(decode(recvdata)))
        # do something
        return gurgle.GURGLE_SUCCESS
    def is_connected(self):
        return self.__is_connected
    def connect_to_server(self,strDomain,nPort,user_name,pass_word):
        if self.is_connected():                             #
            sys.stderr.write('You have already connected to remote,connection was refused!\n')
            return gurgle.GURGLE_ALREADY_CONNECTED
        if strDomain is not None:
            self.set_remote_host(strDomain)
        if nPort is not None:
            self.set_remote_port(nPort)
        if not self.is_remote_addr_set():                   # if the strDomain or nPort is None
            return gurgle.GURGLE_FAILED                     # alrealy set error code in function
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            return gurgle.GURGLE_FAILED                     # no error occupy
        try:
            self.__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as error_message:
            sys.stderr.write('Error creating socket:%s\n' %error_message)
            return gurgle.GURGLE_FAILED_TO_CREATE_SOCKET
        try:
            self.__socket.connect((self.get_remote_host(),self.get_remote_port()))
        except socket.gaierror as error_message:
            sys.stderr.write('Error[gaierror]:%s\n'%error_message)
            return gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
        except socket.error as error_message:
            sys.stderr.write('Error connecting:%s\n'%error_message)
            return gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
        if not (user_name and pass_word):
            sys.stderr.write('Username or password is incorrect!\n')
            return gurgle.GURGLE_FAILED_TO_AUTH
#   Check version
        data = json.dumps('{"id":"%d", "version":"%s"}' % (self.create_id(),self.get_version()))
        if self.send(encode(data)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024)
        if recvdata is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        data = None
        data = json.loads(json.loads(decode(recvdata)))
        if data['version'] != core.get_version():
            sys.stderr.write("Protocol's version do not match!\n")
            self.__socket.close()
            return gurgle.GURGLE_FAILED_TO_AUTH
#   Get authentication method
        data = json.dumps('{"id":"%d", "cmd":"query", "params":{"query":"auth_method"}}'
                %self.create_id())
        if self.send(encode(data)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024)
        if data is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        data = json.loads(json.loads(decode(recvdata)))
        self.__auth_method = data['params']['answer']
        print ('auth method = %s'%self.__auth_method)
        self.ping()
        for method in gurgle.auth_method_supported:
            if method == self.__auth_method:
                return self.do_auth(user_name,pass_word)   # sign in
        sys.stderr.write("Authenticated method [%s] wasn't supported\n" %self.__auth_method)
        return gurgle.GURGLE_FAILED_TO_AUTH

    def disconnect_from_server(self):
        if not self.is_connected:                           # if you did not connected to server
            sys.stderr.write('You have not connected to remote!\n')
            return  gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
        else:
            pass                                            # wait for adding
        return gurgle.GURGLE_SUCCESS


if __name__ == '__main__':
    core = gurgle(gurgle.GURGLE_CLIENT)
    core.set_remote_host('127.0.0.1')
    core.set_remote_port(400097)
    core.connect_to_server('127.0.0.1',40097,'tricks','2341')
