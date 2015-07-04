#!/usr/bin/python3
#!encoding='utf-8'

import socket
import _thread
from protocol.gurgle import *
from codecs import decode, encode
import getpass

if __name__ == '__main__':
    core = gurgle(gurgle.GURGLE_CLIENT)
    domain = input("Where you want to connect?(localhost as default): ")
    if domain == '':
        domain = 'localhost'
    port = input("Port(40097 as default): ")
    if port == '':
        port = 40097
    else:
        port = int(port)
    try:
        core.connect_to_server(domain,40097,300)
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
        message_id = core.create_id()
        command = input("> ")
        if command == '':
            continue
        elif command == 'recv':
            data = core.recv(message_id = 0)
            print ("%s"%data)
            if input("reply?(y/n): ").lower() != 'y':
                continue
            else:
                message_id = data['id'] 
                command = input("> ")
                if command == '':
                    continue
        if command.find(' ') == -1:
            senddata = {
                'id'    : message_id,
                'cmd'   : command
            }
            senddata = json.dumps(senddata)
            core.send(encode(senddata))
            ask = input("Waiting for reply?(yes) ").lower()
            if ask == '' or ask == 'yes':
                buf = core.recv(message_id)
                print ("%s"%buf)
            continue
        (command,nt) = command.split(' ',1)
        if command == None:
            continue
        if nt.find(' ') == -1:
            if nt == 'null':
                obj = None
            else:
                obj = nt
            senddata = {
                'id'    : message_id,
                'cmd'   : command,
                'obj'   : obj,
            }   
            core.send(encode(json.dumps(senddata)))
            ask = input("Waiting for reply?(yes) ").lower()
            if ask == '' or ask == 'yes':
                buf = core.recv(message_id)
                print ("%s"%buf)
            continue
        else:
            (obj,params) = nt.split(' ',1)
        if params.find(' ') == -1:
            (field,value) = params.split('=',1)
            senddata = {
                'id'    : message_id,
                'cmd'   : command,
                'obj'   : obj,
                'params': {
                }
            }   
            senddata['params'][field] = value
            senddata = json.dumps(senddata)
            core.send(encode(senddata))
            ask = input("Waiting for reply?(yes) ").lower()
            if ask == '' or ask == 'yes':
                buf = core.recv(message_id)
                print ("%s"%buf)
            continue
        for i in list(params):
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
            buf = core.recv(message_id)
            print ("%s"%buf)
    core.publish_self_presence_update(last_name = "SternW",first_name="Zhang",status = "Avaliable")
            
