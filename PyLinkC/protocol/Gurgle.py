#!/usr/bin/python3
#!encoding='utf-8'

import socket
import _thread

class Gurgle:
    gurgleVersion = 'Unusable'
    __remoteHost = '127.0.0.1'
    __remotePort = 40097
    def __init__(self):
        print ('Gurgle Version',self.gurgleVersion,'Initlalize..')
    def __del__(self):
        print ('Gurgle Deleting....')
    def SetRemoteAddr(self,strHost,nPort):
        self.__remoteHost = strHost
        self.__remotePort = nPort
    def GetRemoteAddr(self):
        return (__remoteHost)
    def GetRemotePort(self):
        return (__remotePort)

if __name__ == '__main__':
    test = Gurgle('127.0.0.1',40097)
