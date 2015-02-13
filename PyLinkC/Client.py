#!/usr/bin/python3
#!encoding='utf-8'

import socket
import _thread
from protocol.gurgle import *
from codecs import decode, encode

if __name__ == '__main__':
    core = gurgle(gurgle.GURGLE_CLIENT)
    core.set_remote_host('127.0.0.1')
    core.set_remote_port(400097)
    core.connect_to_server('127.0.0.1',40097,'tricks','2341')
    print (core.get_last_error())
