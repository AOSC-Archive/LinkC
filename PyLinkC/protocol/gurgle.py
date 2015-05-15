#!/usr/bin/python3
#!encoding='utf-8'

import socket
import json
import _thread
import random
import sys
from codecs import decode, encode
import string
import time
import datetime

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
    def get_data(self,packageID):
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
    def get_root(self):
        return self.root
    def get_size(self):
        return self.size
    def print_list(self):
        tempNode = self.root
        while tempNode is not None:
            print (tempNode.data)
            tempNode = tempNode.nextNode

class gurgle_protocol_error(Exception):
    pass

class gurgle_network_error(Exception):
    pass

class gurgle_auth_error(Exception):
    pass

class gurgle_params_error(Exception):
    pass

class gurgle:
    auth_method_supported               = ["plain_password"]
    GURGLE_LOG_MODE_ERROR               = 1
    GURGLE_LOG_MODE_COMMON              = 2
    GURGLE_LOG_MODE_DEBUG               = 3
    GURGLE_CLIENT                       = 1
    GURGLE_SERVER                       = 2
    GURGLE_GROUP                        = 3
    GURGLE_FAILED                       = False
    GURGLE_SUCCESS                      = True
    GURGLE_HOST_NOT_SET                 = 10
    GURGLE_PORT_NOT_SET                 = 11
    GURGLE_ALREADY_CONNECTED            = 12
    GURGLE_HAVENT_CONNECTED             = 13
    GURGLE_FAILED_TO_AUTH               = 14
    GURGLE_FAILED_TO_CREATE_SOCKET      = 15
    GURGLE_FAILED_TO_CONNECT_TO_REMOTE  = 16
    GURGLE_FAILED_TO_RECV               = 17
    GURGLE_FAILED_TO_SEND               = 18
    GURGLE_CONNECTION_CLOSED            = 19
    GURGLE_VERSION_DNOT_MATCH           = 20
    def __init__(self,_mode):
        self.__gurgleVersion    = 'Unusable'
        self.__remoteHost       = None
        self.__remotePort       = None
        self.__is_connected     = False
        self.__runtime_mode     = _mode
        self.__socket           = None
        self.__package_list     = packageList()
        self.__auth_method      = 'plain_password'
        self.__terminal_id      = None
        self.__is_authenticated = False
        self.__roster           = None
        self.__roster_etag      = None
        self.__log_level        = 3
        if self.__runtime_mode == gurgle.GURGLE_CLIENT:
            self.write_log ('Gurgle version %s %s'
                    %(self.__gurgleVersion,'initlalized as Client'))
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            self.write_log ('Gurgle version %s %s'
                    %(self.__gurgleVersion,'initlalized as Server'))
        if self.__runtime_mode == gurgle.GURGLE_GROUP:
            self.write_log ('Gurgle version %s %s'
                    %(self.__gurgleVersion,'initlalized as Group server'))
        self.write_log ('Use authenticated method [%s]'
                %self.get_auth_method())
    def __del__(self):
        self.write_log ('Gurgle Deleting....')
        if self.is_connected():
            self.disconnect_from_remote("Porgram was terminated")
        del self.__package_list
    def get_version(self):
        return self.__gurgleVersion;
    def set_socket(self,sockfd):
        self.__socket = sockfd
        self.__is_connected = True
    def create_id(self):
        return random.randint(0,2147483647)
    def create_random_str(self,length):
            return ''.join(random.sample(   \
                string.ascii_letters+string.digits,length))
    def create_terminal_id(self):
        if not self.__terminal_id:
            return ''.join(random.sample(string.ascii_letters+string.digits,8))
        else :
            return self.__terminal_id
    def write_log(self,log,mode = None,l_level = None):
        if mode == None:
            mode = gurgle.GURGLE_LOG_MODE_COMMON
        if l_level != None:
            level = int(l_level)
        else:
            level = self.get_log_level()
        if mode > level:
            return None
        if mode == gurgle.GURGLE_LOG_MODE_ERROR:     # error mode
            sys.stderr.write ("[%s] : %s\n"
                    %(time.asctime(time.localtime()),log))
        elif  mode == gurgle.GURGLE_LOG_MODE_COMMON:      # common mode
            sys.stdout.write ("[%s] : %s\n"
                    %(time.asctime(time.localtime()),log))
        elif mode == gurgle.GURGLE_LOG_MODE_DEBUG:
            sys.stdout.write ("[%s] : %s\n"
                    %(time.asctime(time.localtime()),log))
    def recv(self,buf_size = 512, request_id = 0, timeout = 5, max_try = 2):
        if self.is_connected() == False:
            return None
        data = self.__package_list.get_data(request_id)
        if data is not None:
            return data
        current_try = 0
        self.__socket.settimeout(timeout)
        while current_try < max_try:
            try:
                buf = self.__socket.recv(buf_size)
            except socket.timeout:
                current_try += 1
                continue
            except socket.error as e:
                self.write_log(e,gurgle.GURGLE_LOG_MODE_ERROR)
                raise gurgle_network_error(e)
            if not len(buf):
                self.write_log('Connection was closed by peer')
                self.__is_connected = False
                raise gurgle_network_error(
                        'Connection was unexpectedly closed by peer'
                    )
            buf = json.loads(decode(buf))
            if 'id' not in buf:
                if request_id == 0:
                    return buf
                else:
                    self.__package_list.insert(buf,0)
                    current_try += 1
                    continue
            if 'cmd' in buf:
                if buf['cmd'] == 'kill':
                    error   = "null"
                    reason  = "null"
                    if 'error' in buf:
                        error   = buf['error']
                    if 'reason' in buf:
                        reason  = buf['reason']
                    self.write_log("Connection was closed [%s, %s]"
                            %(error,reason),
                            gurgle.GURGLE_LOG_MODE_ERROR)
                    self.__is_connected = False
                    raise gurgle_network_error(
                            'Connection was closed by peer'
                        )
            if request_id == 0:
                return buf
            if int(buf['id']) == request_id:
                return buf
            else:
                self.__package_list.insert(buf,buf['id'])
                current_try += 1
                continue
        self.write_log("Failed to receive",gurgle.GURGLE_LOG_MODE_ERROR)
        raise gurgle_network_error(
                'Failed to recvice'
            )
    def send(self,buf):
        if self.is_connected() == False:
            raise gurgle_network_error(
                    'Connection has not been established'
                )
        try:
            self.__socket.send(buf)
        except socket.error as e:
            self.write_log(e,gurgle.GURGLE_LOG_MODE_ERROR)
            raise gurgle_network_error(e)
        return gurgle.GURGLE_SUCCESS
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
    def get_auth_method(self):
        return self.__auth_method;
    def get_roster_etag(self):
        return self.__roster_etag
    def set_roster(self):
        pass
    def set_log_level(self,level):
        self.__log_level = int(level)
    def get_log_level(self):
        return self.__log_level
    def is_remote_addr_set(self):
        if not self.__remoteHost:
            self.write_log('Remote addr is not set!'
                    ,gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_HOST_NOT_SET
        if not self.__remotePort:
            self.write_log('Remote port is not set!'
                    ,gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_PORT_NOT_SET
        return gurgle.GURGLE_SUCCESS
    def request_roster(self):
# check whether authenticated
        if not self.is_authenticated(onlineCheck = False):
            if not self.is_authenticated(onlineCheck = False):
                self.write_log("You have not been authenticated!");
                return gurgle.GURGLE_FAILED;
        senddata = None
        request_id = self.create_id()
        rosterETag = self.get_roster_etag()
        if not rosterETag:
        # Directly get the whole of roster
            senddata = json.dumps({
                    "id"    : request_id,
                    "cmd"   : "get_roster",
                })
        else:
        # Use ETag
            senddata = json.dumps({
                    "id"    : request_id,
                    "cmd"   : "get_roster",
                    "params":{
                        "etag"  : rosterETag
                    }
                })
    def update_roster(self):
        pass
    def do_auth(self,ID, password,protocol = 'grgl'):
        status = self.is_authenticated()
        if status == None:
            return gurgle.GURGLE_FAILED_TO_RECV
        if status == True:
            self.write_log("You have already been authenticated")
            return gurgle.GURGLE_SUCCESS
        if not (ID and password):
            self.write_log('Username or password is empty!'
                    ,gurgle.GURGLE_LOG_MODE_ERROR)
            raise gurgle_auth_error(
                    'Username or password is empty'
                )
        if self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            request_id = self.create_id()
            senddata = json.dumps({
                    "id"    :request_id,
                    "cmd"   :"auth",
                    "from"  :"%s:%s"%(protocol,ID), 
                    "params":{
                        "method":self.get_auth_method(),
                        "password":password
                    }
                })
            self.send(encode(senddata))
            recvdata = self.recv(512,request_id)
            if recvdata == None:
                return gurgle.GURGLE_FAILED_TO_RECV
            if 'error' in recvdata:
                if recvdata['error'] == 'null':
                    self.write_log("Auth successfully")
                    self.set_authenticated(True)
                    return gurgle.GURGLE_SUCCESS
                else:
                    self.write_log(
                            "Authenticate Error [%s]"%recvdata['error']
                        )
                    raise gurgle_auth_error(
                            'Authenticate Error [%s]'%recvdata['error']
                        )
            else:
                self.write_log(data)
        else:
            self.write_log("    \
                    Server to server authentication is not supported",
                    gurgle.GURGLE_LOG_MODE_ERROR)
            raise gurgle_auth_error(
                    "Server to server authentication is not supported"
                )
    def is_authenticated(self,onlineCheck = False):
        if self.get_runtime_mode() != gurgle.GURGLE_CLIENT:
            return self.__is_authenticated
        else:
            if not onlineCheck:
                return self.__is_authenticated
            request_id = self.create_id()
            senddata = json.dumps({
                    "id"    : request_id,
                    "cmd"   : "query",
                    "params": {
                        "query" : "auth_status"
                    }
                })
            if self.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
                return gurgle.GURGLE_FAILED_TO_SEND
            recvdata = self.recv(1024,request_id)
            if recvdata is None:
                return gurgle.GURGLE_FAILED_TO_RECV
            if 'params' not in recvdata:
                self.set_authenticated(False)
                return False
            if 'answer' not in recvdata['params']:
                self.set_authenticated(False)
                return False
            if recvdata['params']['answer'] == 'Authenticated':
                self.set_authenticated(True)
                return True
            else:
                self.set_authenticated(False)
                return False
    def set_authenticated(self,Authenticated):
        self.__is_authenticated = Authenticated
    def ping(self):
        request_id = self.create_id()
        senddata = json.dumps({
                "id"    : request_id,
                "cmd"   : "ping"
            })
        preSentTime = datetime.datetime.now().microsecond;
        if self.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024,request_id)
        if recvdata is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        if 'cmd' not in recvdata:
            self.emergency_quit('SyntaxError','Pong without cmd field')
        postSentTime = datetime.datetime.now().microsecond;
        self.write_log("ping......%.2f ms"%((postSentTime - preSentTime)/1000))
        # do something
        return gurgle.GURGLE_SUCCESS
    def check_auth_method(self):
        request_id = self.create_id()
        data = json.dumps({
                "id"    :request_id,
                "cmd"   :"query",
                "params":{
                    "target" : "auth_method"
                }
            })
        if self.send(encode(data)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024,request_id)
        if recvdata is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        self.__auth_method = recvdata['params']['auth_method'][0]
        isAuthenticatedMethodSupported = False
        for method in self.auth_method_supported:
            if method == self.get_auth_method():
                isAuthenticatedMethodSupported = True
        return isAuthenticatedMethodSupported
    def check_version(self):
        request_id = self.create_id()
        data = json.dumps({
                "id"     : request_id,
                "version": self.get_version()
            })
        if self.send(encode(data)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024,request_id)
        if recvdata is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        if recvdata['version'] != self.get_version():
            return gurgle.GURGLE_VERSION_DONOT_MATCH
        return gurgle.GURGLE_SUCCESS
    def get_self_information(self):
        pass
    def publish_presence(self,status = 'avalible', mood = 'null'):
        if not self.authenticated():
            self.write_log("Can't publish presence without been authenticated")
            return gurgle.GURGLE_FAILED
    def is_connected(self):
        return self.__is_connected
    def connect_to_server(self,strDomain,nPort,user_name,pass_word,timeout=5):
        if self.is_connected():                             #
            self.write_log('You have already connected to remote'
                    ,gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_ALREADY_CONNECTED
        if strDomain is not None:
            self.set_remote_host(strDomain)
        if nPort is not None:
            self.set_remote_port(nPort)
        if not self.is_remote_addr_set():
            return gurgle.GURGLE_FAILED
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            return gurgle.GURGLE_FAILED
        if not (user_name and pass_word):
            self.write_log('Username or password is incorrect!',
                    gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_FAILED_TO_AUTH
#   Basic connection
        try:
            self.__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as error_message:
            self.write_log(error_message,
                    gurgle.GURGLE_LOG_MODE_ERROR)
            raise gurgle_network_error(error_message[1])
        self.__socket.settimeout(timeout)
        try:
            self.__socket.connect((
                self.get_remote_host(),
                self.get_remote_port())
            )
        except socket.timeout as error_message:
            self.write_log("Failed to connect [Connection time out]"
                    ,gurgle.GURGLE_LOG_MODE_ERROR)
            raise gurgle_network_error(error_message)
        except socket.gaierror as error_message:
            self.write_log(error_message,
                    gurgle.GURGLE_LOG_MODE_ERROR)
            raise gurgle_network_error(error_message)
        except socket.error as error_message:
            self.write_log(error_message,
                    gurgle.GURGLE_LOG_MODE_ERROR)
            raise gurgle_network_error(error_message)
#   Basic connection was built
        self.__is_connected = True
#   Check version
        if self.check_version() == gurgle.GURGLE_VERSION_DNOT_MATCH:
            self.write_log("Protocol's version do not match!",
                    gurgle.GURGLE_LOG_MODE_ERROR)
            self.disconnect_from_remote()
            return gurgle.GURGLE_VERSION_DNOT_MATCH
#   Ping
        self.ping()
#   Check Authenticated method
        if self.check_auth_method() == False:
            self.write_log("Authenticated method is not supported",
                    gurgle.GURGLE_LOG_MODE_ERROR)
            self.disconnect_from_remote(
                    "Authenticated method is not supported"
                )
            raise gurgle_protocol_error(
                    'Authenticated method is not supported'
                )
#   Auth
        try:
            self.do_auth('%s@%s/%s'
                                    %(user_name,
                                    self.get_remote_host(),
                                    self.create_terminal_id()
                                    ),pass_word
                            )
        except gurgle_auth_error as err:
#   Failed to authenticate
            raise gurgle_auth_error(err)
    def emergency_quit(self,error       =   "UnknownError",     \
                            reason      =   "Unkonwn reason",   \
                            request_id  =   0                   \
                            ):
        cmd = "kill"
        if self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            cmd = "quit"
        senddata = json.dumps({
                "id"    : request_id,
                "cmd"   : cmd,
                "params": {
                    "error"   : error,
                    "reason"  : reason
                }
            })
        self.send(encode(senddata))
        self.__socket.close()
    def disconnect_from_remote(self,reason = "Unkonwn reason"):
        if not self.is_connected():
            self.write_log('You have not connected to remote!'
                    ,gurgle.GURGLE_LOG_MODE_ERROR)
            return  gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
        elif self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            senddata = json.dumps({
                    "id"    : request_id,
                    "cmd"   : "quit",
                    "params": {
                        "reason" : reason
                    }
                })
            self.send(encode(senddata))
            recvdata = self.recv(512)
            if recvdata == None:
                self.write_log("Connection was closed by peer"
                        ,gurgle.GURGLE_LOG_MODE_ERROR)
                self.__socket.close()
                return gurgle.GURGLE_SUCCESS
            if not 'cmd' in recvdata:
                self.write_log('Some errors occupied,Just quit'
                        ,gurgle.GURGLE_LOG_MODE_ERROR)
            else:
                self.write_log('server replied %s'%recvdata['cmd'])
        self.__socket.close()
        self.__is_connected = False
        return gurgle.GURGLE_SUCCESS


if __name__ == '__main__':
    core = gurgle(gurgle.GURGLE_CLIENT)
    try:
        core.connect_to_server('127.0.0.1',40097,'tricks','123321123')
    except gurgle_network_error:
        pass
    except gurgle_protocol_error:
        pass
    except gurgle_auth_error:
        pass
