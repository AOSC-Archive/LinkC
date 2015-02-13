#!/usr/bin/python3
#!encoding='utf-8'

import socket
import json
import _thread
from codecs import decode, encode

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
    GURGLE_CLIENT                       = 1
    GURGLE_SERVER                       = 2
    GURGLE_GROUP                        = 3
    GURGLE_FAILED                       = False
    GURGLE_SUCCESS                      = True
    GURGLE_HOST_NOT_SET                 = 10
    GURGLE_PORT_NOT_SET                 = 11
    GURGLE_ALREADY_CONNECTED            = 12
    GURGLE_HAVENT_CONNECTED             = 13
    GURGLE_FAILED_TO_LOGIN              = 14
    GURGLE_FAILED_TO_CREATE_SOCKET      = 15
    GURGLE_FAILED_TO_CONNECT_TO_REMOTE  = 16
    def __init__(self,_mode):
        self.__gurgleVersion= 'Unusable'
        self.__remoteHost       = None
        self.__remotePort       = None
        self.__is_connected     = False
        self.last_error         = None
        self.last_error_code    = None
        self.__runtime_mode     = _mode
        self.socket             = None
        self.__packageList      = packageList();
        self.__current_id       = 0
        if self.__runtime_mode == gurgle.GURGLE_CLIENT:
            print ('Gurgle version',self.__gurgleVersion,'initlalized as Client')
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            print ('Gurgle version',self.__gurgleVersion,'initlalized as Server')
        if self.__runtime_mode == gurgle.GURGLE_GROUP:
            print ('Gurgle version',self.__gurgleVersion,'initlalized as Group server')
    def __del__(self):
        print ('Gurgle Deleting....')
    def get_version(self):
        return self.__gurgleVersion;
    def create_id(self):
        self.__current_id += 1
        return self.__current_id;
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
    def get_last_error(self):
        returnStrVar = self.last_error
        returnIntVar = self.last_error_code
        self.last_error = 'success'
        self.last_error_code = gurgle.GURGLE_SUCCESS
        return (returnStrVar,returnIntVar)
    def is_remote_addr_set(self):
        if not self.__remoteHost:
            self.last_error = 'Remote addr is not set!'
            self.last_error_code = gurgle.GURGLE_HOST_NOT_SET
            return gurgle.GURGLE_FAILED
        if not self.__remotePort:
            self.last_error = 'Remote port is not set!'
            self.last_error_code = gurgle.GURGLE_PORT_NOT_SET
            return gurgle.GURGLE_FAILED
        return gurgle.GURGLE_SUCCESS
    def is_connected(self):
        return self.__is_connected
    def connect_to_server(self,strDomain,nPort,user_name,pass_word):
        if self.is_connected():                             #
            self.last_error = 'You have already connected to remote,connection was refused!'
            self.last_error_code = gurgle.GURGLE_ALREADY_CONNECTED
            return gurgle.GURGLE_FAILED
        if strDomain is not None:
            self.set_remote_host(strDomain)
        if nPort is not None:
            self.set_remote_port(nPort)
        if not self.is_remote_addr_set():                   # if the strDomain or nPort is None
            return gurgle.GURGLE_FAILED                     # alrealy set error code in function
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            return gurgle.GURGLE_FAILED                     # no error occupy
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as error_message:
            self.last_error = error_message
            self.last_error_code = gurgle.GURGLE_FAILED_TO_CREATE_SOCKET
            return gurgle.GURGLE_FAILED
        try:
            self.socket.connect((self.get_remote_host(),self.get_remote_port()))
        except socket.gaierror as error_message:
            self.last_error = error_message
            self.last_error_code = gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
            return gurgle.GURGLE_FAILED
        except socket.error as error_message:
            self.last_error = error_message
            self.last_error_code = gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
            return gurgle.GURGLE_FAILED
        if not (user_name and pass_word):
            self.last_error = 'Username or password is incorrect!'
            self.last_error_code = gurgle.GURGLE_FAILED_TO_LOGIN
            return gurgle.GURGLE_FAILED
#   Check version
        data = json.dumps('{"id":"%d", "version":"%s"}' % (self.create_id(),self.get_version()))
        try:
            self.socket.send(encode(data))
        except socket.error as e:
            print('Error sending data:%s' %e)
            return gurgle.GURGLE_FAILED
        return gurgle.GURGLE_SUCCESS

    def disconnect_from_server(self):
        if not self.is_connected:                           # if you do not connected to server
            self.last_error = 'You have not connected to remote!'
            self.last_error_code = gurgle.GURGLE_HAVENT_CONNECTED
            return gurgle.GURGLE_FAILED
        else:
            pass                                            # wait for adding
        return gurgle.GURGLE_SUCCESS


if __name__ == '__main__':
    core = gurgle(gurgle.GURGLE_CLIENT)
    core.set_remote_host('127.0.0.1')
    core.set_remote_port(400097)
    core.connect_to_server('127.0.0.1',40097,'tricks','2341')
    print (core.get_last_error())
