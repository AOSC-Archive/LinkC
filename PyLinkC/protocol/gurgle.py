#!/usr/bin/python3
#!encoding='utf-8'

import socket
import json
import _thread
import random
import sys
from codecs import decode, encode
import string
import time

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
    encrypted_method_supported          = ["plain"]
    GURGLE_LOG_MODE_DEBUG               = 0
    GURGLE_LOG_MODE_COMMON              = 1
    GURGLE_LOG_MODE_ERROR               = 2
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
        self.__encrypted_method = 'plain'
        self.__terminal_id      = None
        if self.__runtime_mode == gurgle.GURGLE_CLIENT:
            self.write_log ('Gurgle version %s %s'%(self.__gurgleVersion,'initlalized as Client'))
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            self.write_log ('Gurgle version %s %s'%(self.__gurgleVersion,'initlalized as Server'))
        if self.__runtime_mode == gurgle.GURGLE_GROUP:
            self.write_log ('Gurgle version %s %s'%(self.__gurgleVersion,'initlalized as Group server'))
        self.write_log ('Use encrypted method [%s] by default' %self.get_encrypted_method())
    def __del__(self):
        self.write_log ('Gurgle Deleting....')
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
    def write_log(self,log,mode = None):
        if mode == None:
            mode = gurgle.GURGLE_LOG_MODE_COMMON
        if  mode == gurgle.GURGLE_LOG_MODE_COMMON:      # common mode
            sys.stdout.write ("[%s] : %s\n"%(time.asctime(time.localtime()),log))
        elif mode == gurgle.GURGLE_LOG_MODE_ERROR:     # error mode
            sys.stderr.write ("[%s] : %s\n"%(time.asctime(time.localtime()),log))
        elif mode == gurgle.GURGLE_LOG_MODE_DEBUG:
            sys.stdout.write ("[%s] : %s\n"%(time.asctime(time.localtime()),log))
    def recv(self,bufsize):
        try:
            buf = self.__socket.recv(bufsize)
        except socket.error as e:
            self.write_log(e,gurgle.GURGLE_LOG_MODE_ERROR)
            return None
        if not len(buf):
            self.write_log('Connection closed',gurgle.GURGLE_LOG_MODE_ERROR)
            return None
        return buf
    def send(self,buf):
        try:
            self.__socket.send(buf)
        except socket.error as e:
            self.write_log(e,gurgle.GURGLE_LOG_MODE_ERROR)
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
    def get_encrypted_method(self):
        return self.__encrypted_method;
    def is_remote_addr_set(self):
        if not self.__remoteHost:
            self.write_log('Remote addr is not set!',gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_HOST_NOT_SET
        if not self.__remotePort:
            self.write_log('Remote port is not set!',gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_PORT_NOT_SET
        return gurgle.GURGLE_SUCCESS
    def do_auth(self,ID, password,protocol = 'grgl'):
        if not (ID and password):
            self.write_log('ID or password is empty!',gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_FAILED_TO_AUTH
        if self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            senddata = json.dumps('{"id":"%d", "cmd":"auth", "from":"%s","params":{"method":"%s","password":"%s"}}'
                    %(self.create_id(),
                        "%s:%s"%(protocol,ID),
                        self.get_encrypted_method(),password))
            self.send(encode(senddata))
            recvdata = self.recv(512)
            data = json.loads(json.loads(decode(recvdata)))
            if 'error' in data:
                if data['error'] == 'null':
                    self.write_log("Auth successfully")
                else:
                    self.write_log("Auth Error[%s]"%data['error'])
            else:
                self.write_log(data)
            self.disconnect_from_remote()
        else:
            pass
    def ping(self):
        self.write_log('ping',gurgle.GURGLE_LOG_MODE_DEBUG)
        senddata = json.dumps('{"id":"%d", "cmd":"ping"}'
                %self.create_id())
        if self.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024)
        if recvdata is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        data = json.loads(json.loads(decode(recvdata)))
        if 'cmd' not in data:
            self.emergency_quit('SyntaxError','Pong without cmd field')
        self.write_log('%s'%data['cmd'],gurgle.GURGLE_LOG_MODE_DEBUG)
        # do something
        return gurgle.GURGLE_SUCCESS
    def is_connected(self):
        return self.__is_connected
    def connect_to_server(self,strDomain,nPort,user_name,pass_word):
        if self.is_connected():                             #
            self.write_log('You have already connected to remote,connection was refused!'
                    ,gurgle.GURGLE_LOG_MODE_ERROR)
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
            self.write_log(error_message,gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_FAILED_TO_CREATE_SOCKET
        try:
            self.__socket.connect((self.get_remote_host(),self.get_remote_port()))
        except socket.gaierror as error_message:
            self.write_log(error_message,gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
        except socket.error as error_message:
            self.write_log(error_message,gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
        if not (user_name and pass_word):
            self.write_log('Username or password is incorrect!',gurgle.GURGLE_LOG_MODE_ERROR)
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
            self.write_log("Protocol's version do not match!",gurgle.GURGLE_LOG_MODE_ERROR)
            self.__socket.close()
            return gurgle.GURGLE_FAILED_TO_AUTH
#   Get authentication method
        data = json.dumps('{"id":"%d", "cmd":"query", "params":{"query":"encrypted_method"}}'
                %self.create_id())
        if self.send(encode(data)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024)
        if data is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        data = json.loads(json.loads(decode(recvdata)))
        self.__encrypted_method = data['params']['answer']
        self.write_log ('encrypted method = %s'%self.__encrypted_method, 0)
        self.ping()
        self.__is_connected = True
        for method in gurgle.encrypted_method_supported:
            if method == self.__encrypted_method:
                return self.do_auth('%s@%s/%s'
                        %(user_name,self.get_remote_host(),self.create_terminal_id()),
                        pass_word)
        self.write_log("Authenticated method [%s] wasn't supported"
                %self.__encrypted_method,gurgle.GURGLE_LOG_MODE_ERROR)
        self.disconnect_from_remote()
        return gurgle.GURGLE_FAILED_TO_AUTH

    def emergency_quit(self,error='UnknownError',reason="Some errors occupied"):
        cmd = "kill"
        if self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            cmd = "quit"
        senddata = json.dumps('{"id":"%d", "cmd":"%s", "error":"%s", "reason":"%s"}'
                %(self.create_id(),cmd,error,reason))
        self.send(encode(senddata))
        self.__socket.close()
    def disconnect_from_remote(self,reason = "I just wanna to quit"):
        if not self.is_connected():               # if you did not connected to server
            self.write_log('You have not connected to remote!',gurgle.GURGLE_LOG_MODE_ERROR)
            return  gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
        elif self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            senddata = json.dumps('{"id":"%d", "cmd":"quit","reason":"%s"}'
                    %(self.create_id(),reason))
            self.send(encode(senddata))
            recvdata = self.recv(512)
            recvdata = json.loads(json.loads(decode(recvdata)))
            if not 'cmd' in recvdata:
                self.write_log('Some errors occupied,Just quit',gurgle.GURGLE_LOG_MODE_ERROR)
            else:
                self.write_log('server replied %s'%recvdata['cmd'])
        else:
            senddata = json.dumps('{"id":"%d","cmd":"bye"}')
            self.send(encode(senddata))
        self.__socket.close()
        return gurgle.GURGLE_SUCCESS


if __name__ == '__main__':
    core = gurgle(gurgle.GURGLE_CLIENT)
    core.set_remote_host('127.0.0.1')
    core.set_remote_port(400097)
    core.connect_to_server('127.0.0.1',40097,'tricks','123321123')
