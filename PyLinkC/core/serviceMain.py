#!/usr/bin/python3
#!encoding='utf-8'

import sys
import threading
sys.path.append("..")
from protocol.Gurgle import *

class serviceMain(threading.Thread):
    Core = Gurgle()
    def __init__(self):
        pass
    def __del__(self):
        del Core
    def SetRemoteAddr(self,strAddr,nPort):
        self.Core.SetRemoteAddr(strAddr,nPort)
    def run(self):
        pass

if __name__ == '__main__':
    test = serviceMain()
    test.SetRemoteAddr('127.0.0.1',2341)
    del test
