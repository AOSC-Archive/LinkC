#!/usr/bin/python3
#!encoding='utf-8'

import socket
import _thread
from protocol.gurgle import *
from codecs import decode, encode

if __name__ == '__main__':
    core = gurgle(gurgle.GURGLE_CLIENT)
    try:
        core.connect_to_server('127.0.0.1',40097)
    except gurgle_network_error:
        pass
    except gurgle_protocol_error:
        pass
    #   Auth
    try:
        core.plain_password_auth('%s@%s/%s'
                        %('tricks',
                        '127.0.0.1',
                        core.create_terminal_id()
                        ),'123321123'
                    )
    except gurgle_auth_error as err:
#   Failed to authenticate
        core.write_log("Auth failed")
        exit()
    core.write_log("Auth succeed")