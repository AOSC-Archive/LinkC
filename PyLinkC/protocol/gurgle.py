#!/usr/bin/python3
#!encoding='utf-8'

import socket
import json
import _thread

class gurgle:
    GURGLE_CLIENT                       = 1
    GURGLE_SERVER                       = 2
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
        if self.__runtime_mode == gurgle.GURGLE_CLIENT:
            print ('Gurgle version',self.__gurgleVersion,'initlalize as Client')
        else:
            print ('Gurgle version',self.__gurgleVersion,'initlalize as Server')
    def __del__(self):
        print ('Gurgle Deleting....')
    def set_remote_host(self,strHost):
        self.__remoteHost = strHost
    def set_remote_port(self,nPort):
        self.__remotePort = nPort
    def get_runtime_mode(self):
        return self.__runtime_mode
    def get_remote_host(self):
        return (__remoteHost)
    def get_remote_port(self):
        return (__remotePort)
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
        return __is_connected
    def connect_to_server(self,strDomain,nPort,user_name,pass_word):
        self.set_remote_host(strDomain)
        self.set_remote_port(nPort)
        if not is_remote_addr_set():                        # if the strDomain or nPort is None
            return gurgle.GURGLE_FAILED                     # alrealy set error code in function
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            return gurgle.GURGLE_FAILED                     # no error occupy
        if self.is_connected():
            self.last_error = 'You have already connected to remote,connection was refused!'
            self.last_error_code = gurgle.GURGLE_ALREADY_CONNECTED
            return gurgle.GURGLE_FAILED
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as error_message:
            self.last_error = error_message
            self.last_error_code = gurgle.GURGLE_FAILED_TO_CREATE_SOCKET
            return gurgle.GURGLE_FAILED
        try:
            self.socket.connect((self.get_remote_host,self.get_remote_port))
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
    print (core.get_last_error())
