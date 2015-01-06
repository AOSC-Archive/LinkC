#!/usr/bin/python3
#!encoding='utf-8'

import sys
import threading
sys.path.append("..")
from protocol.Gurgle import *

class serviceMain(threading.Thread):
    Socket = None
    def __init__(self):
        threading.Thread.__init__(self)
        self.Core = Gurgle()
    def __del__(self):
        del self.Core
    def SetRemoteAddr(self,strAddr,nPort,nSocket):
        self.Core.SetRemoteHost(strAddr)
        self.Core.SetRemotePort(nPort)
    def run(self):
        if self.Core.isRemoteAddrSet() == False :
            return

if __name__ == '__main__':
    test = serviceMain()
    test.SetRemoteAddr('127.0.0.1',2341)
    test.start()
