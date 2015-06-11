#!/usr/bin/python3
#!encoding='utf-8'

import socket
import _thread
from protocol.gurgle import *
from codecs import decode, encode
import getpass

if __name__ == '__main__':
    core = gurgle(gurgle.GURGLE_CLIENT)
    domain = input("Where you want to connect?: ")
    port = input("Port(40097 as default): ")
    if port == '':
        port = 40097
    else:
        port = int(port)
    try:
        core.connect_to_server(domain,40097)
    except gurgle_network_error:
        del core
        exit()
    except gurgle_protocol_error:
        del core
        exit()
    #   Auth
    username = input("Input your username: ")
    password = getpass.getpass("Input your password: ")
    try:
        core.plain_password_auth('%s'%core.make_up_full_id(username,domain,core.create_terminal_id()),password)
    except gurgle_auth_error as err:
#   Failed to authenticate
        core.write_log("Auth failed")
        del core
        exit()
    except gurgle_network_error as e:
        core.write_log("Network error:%s"%e)
        del core
        exit()
    core.write_log("Auth succeed")
    command = ""
    while True:
        flag = True
        request_id = core.create_id()
        command = input("> ")
        if command == '':
            continue
        if not command.find(' '):
            senddata = {
                'id'    : request_id,
                'cmd'   : command
            }
            senddata = json.dumps(senddata)
            core.send(encode(senddata))
            ask = input("Waiting for reply?(yes) ").lower()
            if ask == '' or ask == 'yes':
                buf = core.recv()
                print ("%s"%buf)
            continue
        (command,params) = command.split(' ',1)
        if command == None:
            continue
        params = params.split(' ')
        senddata = {
            'id'    : request_id,
            'cmd'   : command,
            'params'    : {}
        }
        for i in params:
            tVar=str(i).split("=",1)
            if tVar == None or tVar[0] == None:
                print ("SyntaxError in %s"%i)
                flag = False
                break
            (field,value) = tVar
            if field == None or value == None:
                print ("SyntaxError in %s"%i)
                flag = False
                break
            senddata['params'][field] = value
        if flag == False:
            continue
        senddata = json.dumps(senddata)
        core.send(encode(senddata))
        ask = input("Waiting for reply?(yes) ").lower()
        if ask == '' or ask == 'yes':
            buf = core.recv(request_id = request_id)
            print ("%s"%buf)
    core.publish_self_presence_update(last_name = "SternW",first_name="Zhang",status = "Avaliable")
            
