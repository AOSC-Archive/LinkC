#!/usr/bin/python3
#!encoding='utf-8'

import socket
import _thread

class GurgleError:
    GURGLE_FAILED       = False;
    GURGLE_SUCCESS      = True;
    GURGLE_HOST_NOT_SET = 10
    GURGLE_PORT_NOT_SET = 11
    def __init__(self):
        pass
    def __del__(self):
        pass


class Gurgle:
    gurgleVersion = 'Unusable'
    __remoteHost = None
    __remotePort = 'success'
    LastError    = None
    LastErrorCode= 0
    def __init__(self):
        print ('Gurgle Version',self.gurgleVersion,'Initlalize..')
    def __del__(self):
        print ('Gurgle Deleting....')
    def SetRemoteHost(self,strHost):
        self.__remoteHost = strHost
    def SetRemotePort(self,nPort):
        self.__remotePort = nPort
    def GetRemoteHost(self):
        return (__remoteHost)
    def GetRemotePort(self):
        return (__remotePort)
    def GetLastError(self):
        returnStrVar = self.LastError
        returnIntVar = self.LastErrorCode
        self.LastError = 'success'
        self.LastErrorCode = GurgleError.GURGLE_SUCCESS
        return (returnStrVar,returnIntVar)
    def isRemoteAddrSet(self):
        if not self.__remoteHost:
            self.LastError = 'Remote addr is not set!'
            self.LastErrorCode = GurgleError.GURGLE_HOST_NOT_SET
            return False
        if not self.__remotePort:
            self.LastError = 'Remote port is not set!'
            self.LastErrorCode = GurgleError.GURGLE_PORT_NOT_SET
            return False
        return True
    def ConnectToServer(self):
        if not isRemoteAddrSet():
            print ('Failed to connect to server!')

if __name__ == '__main__':
    Core = Gurgle()
    Core.SetRemoteHost('127.0.0.1')
    Core.SetRemotePort(400097)
    print (Core.GetLastError())
