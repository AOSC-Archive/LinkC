#!/usr/bin/python3
#!encoding='utf-8'

import socket
import json
import _thread
import threading        # for mutex
import random
import sys
from codecs import decode, encode
import string
import time
import datetime

class packageNode:
    nextNode    = None
    data        = None
    ext         = None;
    packageID   = 0
    def __init__(self,nodeData):
        self.data = nodeData

class packageList:
    def __init__(self):
        self.size = 0
        self.root = packageNode()
        self.root = None
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
    def insert(self,newData,packageID,packageExt):
        newNode = packageNode(newData)
        newNode.packageID = packageID
        newNode.ext       = packageExt
        if self.root is None:
            self.root = newNode
            self.size += 1
            return
        tempNode = self.root
        while tempNode.nextNode is not None:
            tempNode = tempNode.nextNode
        tempNode.nextNode = newNode
        self.size += 1
    def get_data(self,packageID,packageExt = None,domain = None,ip = None):
        if self.size == 0:
            return None
        else:
            tempNode = self.root
            for i in range(0,self.size):
                if tempNode.packageID == packageID:
                    if packageID == 0:
                        if gurgle.is_id_match(gurgle,packageExt,tempNode.ext,domain,ip):
                            return tempNode.data
                    else:
                        return tempNode.data
                tempNode = tempNode.nextNode
            return None
    def remove(self,packageID,packageExt=None):
        if self.root is None:
            return False
        if self.root.packageID == packageID:
            if packageID != 0:
                tempNode = self.root.nextNode
                self.root = None
                self.size -= 1
                self.root = tempNode
                return True
            else:
                if packageExt != None:
                    if self.root.ext == packageExt:
                        tempNode = self.root.nextNode
                        self.root = None
                        self.size -= 1
                        self.root = tempNode
                        return True
                    else:
                        pass
                else:
                    tempNode = self.root.nextNode
                    self.root = None
                    self.size -= 1
                    self.root = tempNode
                    return True
        curNode = self.root
        while curNode.nextNode is not None:
            if curNode.nextNode.packageID == packageID:
                tempNode = curNode.nextNode
                curNode.nextNode = curNode.nextNode.nextNode
                tempNode = None
                self.size -= 1
                return True
            elif packageID == 0:
                if packageExt != None:
                    if curNode.nextNode.ext == packageExt:
                        tempNode = curNode.nextNode
                        curNode.nextNode = curNode.nextNode.nextNode
                        tempNode = None
                        self.size -= 1
                        return True
                else:
                    tempNode = curNode.nextNode
                    curNode.nextNode = curNode.nextNode.nextNode
                    tempNode = None
                    self.size -= 1
                    return True
            curNode = curNode.nextNode
        return False
    def get_root(self):
        return self.root
    def get_size(self):
        return self.size
    def print_list(self):
        tempNode = self.root
        while tempNode is not None:
            print (tempNode.data)
            tempNode = tempNode.nextNode

class door_lock():
    def __init__(self):
        self.__lock     = threading.Lock()
    def __del__(self):
        del self.__lock
    def door_open(self):
        return self.__lock.release()
    def door_close(self):
        return self.__lock.acquire()
    def door_rush_into(self):
        if self.__lock.acquire():
            self.__lock.release()
            return True
        else:
            return False
    def door_step_into(self):
        self.__lock.acquire()
    def door_step_out(self):
        self.__lock.release()

class gurgle_protocol_error(Exception):
    pass

class gurgle_network_error(Exception):
    pass

class gurgle_auth_error(Exception):
    pass

class gurgle_params_error(Exception):
    pass

class gurgle:
    auth_method_supported               = ['plain_password']
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
    GURGLE_STATUS_SUPPORTED             = ['avaliable','away','dnd','invisible']
    passwordAllowed = [ 'a','b','c','d','e','f','g','h','i','j','k','l','m',
                        'n','o','p','q','r','s','t','u','v','w','x','y','z',
                        'A','B','C','D','E','F','G','H','I','J','K','L','M',
                        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                        '!','@','#','$','%','^','&','*','-','_','=','<','>',
                        '+','?','/','1','2','3','4','5','6','7','8','9','0']

    usernameAllowed = [ 'a','b','c','d','e','f','g','h','i','j','k','l','m',
                        'n','o','p','q','r','s','t','u','v','w','x','y','z',
                        'A','B','C','D','E','F','G','H','I','J','K','L','M',
                        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                        '+','-','_','1','2','3','4','5','6','7','8','9','0']
    def __init__(self,_mode):
        self.__gurgleVersion    = 'Unusable'
        self.__remoteHost       = None
        self.__remotePort       = None
        self.__is_connected     = False
        self.__runtime_mode     = _mode
        self.__socket           = None
        self.__package_list     = packageList()
        self.__auth_method      = ['plain_password']
        self.__terminal_id      = None
        self.__is_authenticated = False
        self.__roster           = None
        self.__roster_etag      = None
        self.__log_level        = 3
        self.__send_mutex       = threading.Lock()
        self.__recv_mutex       = threading.Lock()
        self.__recv_door_1      = door_lock()
        self.__recv_door_2      = door_lock()
        self.__recv_roster      = 0
        self.__recv_door_2.door_close()
        self.__gurgleId         = None
        self.__session          = None
        if self.__runtime_mode == gurgle.GURGLE_CLIENT:
            self.write_log ('Gurgle version %s %s'
                    %(self.__gurgleVersion,'initlalized as Client'))
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            self.write_log ('Gurgle version %s %s'
                    %(self.__gurgleVersion,'initlalized as Server'))
        if self.__runtime_mode == gurgle.GURGLE_GROUP:
            self.write_log ('Gurgle version %s %s'
                    %(self.__gurgleVersion,'initlalized as Group server'))
    def __del__(self):
        self.write_log ('Gurgle Deleting....')
        if self.is_connected():
            self.disconnect_from_remote("Porgram was terminated")
        del self.__recv_door_1
        del self.__recv_door_2
        del self.__recv_mutex
        del self.__send_mutex
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
            mode = self.GURGLE_LOG_MODE_COMMON
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
    def __recv_lock_release(self):
        self.__recv_mutex.release()
        self.__recv_door_1.door_close()
        self.__recv_door_2.door_open()
        while self.__recv_roster:
            self.__recv_door_2.door_step_into()
        self.__recv_door_2.door_close()
        self.__recv_door_1.door_open()
    def recv(self,buf_size = 512, message_id = 0,message_obj = None, timeout = 5, max_try = 2):
        if self.is_connected() == False:
            return None
        buf = None
        id = 0;
        while(1):
            self.__recv_door_1.door_rush_into()
            if self.__recv_mutex.acquire(blocking=False):
                break
            self.__recv_roaster += 1
            self.__recv_door_2.door_step_into()
            data = self.__package_list.get_data(message_id)
            self.__recv_roaster -= 1
            self.__recv_door_2.door_step_out()
            if data is not None:
                self.__package_list.remove(message_id,message_obj)
                return data
            if timeout == 0:
                return None
            ## status :: door_1 closed, door_2 opened, recv_mutex locked
        if not self.is_connected():
            self.__recv_lock_release()
            raise gurgle_network_error("Connection has not been established!")
        self.__socket.settimeout(timeout)
        return_data = None
        obj = None;
        while True:
            data = self.__package_list.get_data(message_id)
            if data is not None:
                self.__recv_lock_release()
                return data
            if timeout == 0:
                self.__recv_lock_release()
                return None
            buf = None
            try:
                buf = self.__socket.recv(buf_size)
            except socket.timeout:
                continue
            except socket.error as e:
                self.write_log(e,gurgle.GURGLE_LOG_MODE_ERROR)
                self.__recv_lock_release()
                raise gurgle_network_error(e)
            if not len(buf):
                self.write_log('Connection was closed by peer')
                self.__is_connected = False
                self.__is_authenticated = False
                self.__recv_lock_release()
                raise gurgle_network_error(
                        'Connection was unexpectedly closed by peer'
                    )
            buf     = buf.decode()
            start   = 0
            location= 0
            count   = 0
            zBuf    = []
            return_buf = None
            for ch in buf:
                if ch == "{":
                    if count == 0:
                        start = location
                    count += 1
                if ch == "}":
                    count -= 1
                    if count == 0:
                        zBuf.append(buf[start:location+1])
                location += 1
            for buf in zBuf:
                try:
                    buf = json.loads(buf)
                except ValueError as e:
                    self.write_log(e,gurgle.GURGLE_LOG_MODE_ERROR)
                    self.write_log("Error package = [%s]"%buf)
                    continue
                if 'id' not in buf:
                    if message_id == 0:
                        return_buf = buf
                        continue
                else:
                    id = int(buf['id'])
                if 'cmd' in buf:
                    if buf['cmd'] == 'kill':
                        error   = None
                        reason  = None
                        if 'params' in buf:
                            if 'error' in buf['params']:
                                error   = str(buf['params']['error'])
                            if 'reason' in buf['params']:
                                reason  = str(buf['params']['reason'])
                        self.write_log("Connection was closed [%s, %s]"
                                %(error,reason),
                                gurgle.GURGLE_LOG_MODE_ERROR)
                        self.__is_connected = False
                        self.__is_authenticated = False
                        self.__recv_lock_release()
                        raise gurgle_network_error(
                                'Connection was closed by peer'
                            )
                if 'obj' in buf:
                    obj = buf['obj']
                if message_id == 0:
                    if return_buf != None:
                        self.__package_list.insert(buf,id)
                        continue
                    return_buf = buf
                    continue
                if id == message_id:
                    if return_buf != None:
                        self.write_log("WHAT HAPPEDN?")
                    return_buf = buf
                    continue
                else:
                    self.__package_list.insert(buf,id)
            if return_buf != None:
                self.__recv_lock_release()
                return return_buf
    def send(self,buf):
        if self.is_connected() == False:
            raise gurgle_network_error('Connection has not been established')
        self.__send_mutex.acquire()
        try:
            self.__socket.send(buf)
        except socket.error as e:
            self.write_log(e,gurgle.GURGLE_LOG_MODE_ERROR)
            self.__send_mutex.release()
            raise gurgle_network_error(e)
        self.__send_mutex.release()
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
    def get_session(self):
        return self.__session
    def get_log_level(self):
        return self.__log_level
    def get_self_gurgle_id(self):
        if self.__is_authenticated:
            return self.__gurgleId
        return None
    def is_username_acceptable(self,username):
        if username == None:
            return False
        if type(username) != str:
            return False
        for ch in username:
            isFound = False
            for a in self.usernameAllowed:
                if a == ch:
                    isFound = True
                    break
            if isFound == False:
                return False
        return True
    def is_password_acceptabel(self,password):
        if password == None:
            return False
        if type(password) != str:
            return False
        for ch in password:
            isFound = False
            for a in self.usernameAllowed:
                if a == ch:
                    isFound = True
                    break
            if isFound == False:
                return False
        return True
    def analyse_full_id(self,FullSignInID):
        if FullSignInID == None:
            return None
        try:
            (protocol,ID)   = FullSignInID.split(':',1)
        except ValueError as err:
            return None
        if ID.find("@") == -1:
            return None
        (username,suffix) = ID.split("@",1)
        if (username == None) or (suffix == None):
            return None
        if suffix.find("/") == -1:
            return (protocol,username,suffix,None)
        else:
            (domain,terminal) = suffix.split("/",1)
            if domain == None:
                return None
            return (protocol,username,domain,terminal)
    def make_up_full_id(self,username,domain,terminal = None):
        if self.is_username_acceptable(username) == False:
            return None
        if (username == None) or (domain == None):
            return 'SyntaxError'
        if terminal == None:
            return "grgl:%s@%s"%(username,domain)
        return "grgl:%s@%s/%s"%(username,domain,terminal)
    def is_id_match(self,user_id_a,user_id_b,domain,ip):
        if user_id_a == None or user_id_b == None:
            return False
        tmpVarA = self.analyse_full_id(user_id_a)
        if tmpVarA == None:
            return False
        tmpVarB = self.analyse_full_id(user_id_b)
        if tmpVarB == None:
            return False
        if domain == None and ip == None:
            return False
        if domain != None and ip != None:
            if tmpVarA[1] != tmpVarB[1]:
                return False
            if tmpVarA[2] != tmpVarB[2]:
                if tmpVarA[2] == domain:
                    if tmpVarB[2] == ip:
                        return True
                    else:
                        return False
                if tmpVarA[2] == ip:
                    if tmpVarB[2] == domain:
                        return True
                    else:
                        return False
            else:
                return True
        return False
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
            if not self.is_authenticated(onlineCheck = True):
                self.write_log("You have not been authenticated!");
                return gurgle.GURGLE_FAILED;
        senddata = None
        message_id = self.create_id()
        rosterETag = self.get_roster_etag()
        if not rosterETag:
        # Directly get the whole of roster
            senddata = json.dumps({
                    "id"    : message_id,
                    "cmd"   : "get_roster",
                })
        else:
        # Use ETag
            senddata = json.dumps({
                    "id"    : message_id,
                    "cmd"   : "get_roster",
                    "params":{
                        "etag"  : rosterETag
                    }
                })
    def query_roster_update(self,user_id_1,user_id_2,domain,ip):
        pass
    def push_roster_update(self,params):
        senddata = {
            "id"    : self.create_id(),
            "cmd"   : "update",
            "obj"   : "roster",
            "params": params
        }
        try:
            self.send(encode(json.dumps(senddata)))
        except ValueError as err:
            raise ValueError(err)
        except gurgle_network_error as err:
            raise gurgle_auth_error(err)
        return
    def plain_password_auth(self,ID, password,protocol = 'grgl'):
        try:
            status = self.is_authenticated()
        except gurgle_network_error as e:
            raise gurgle_network_error(e)
        if status == True:
            self.write_log("You have already been authenticated")
            return gurgle.GURGLE_SUCCESS
        if not (ID and password):
            self.write_log('Username or password is empty!',gurgle.GURGLE_LOG_MODE_ERROR)
            raise gurgle_auth_error('Username or password is empty')
        if self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            message_id = self.create_id()
            senddata = json.dumps({
                    "id"    :message_id,
                    "cmd"   :"auth",
                    "from"  :ID,
                    "params":{
                        "method"    :   self.get_auth_method(),
                        "password"  :   password
                    }
                })
            self.send(encode(senddata))
            try:
                recvdata = self.recv(512,message_id)
            except gurgle_network_error as e:
                raise gurgle_network_error(e)
            if 'reply' not in recvdata:
                self.write_log("NO REPLY")
                self.__is_authenticated = False
                return gurgle.GURGLE_FAILED
            if 'error' in recvdata['reply']:
                if recvdata['reply']['error'] == None:
                    self.set_authenticated(True)
                    self.__gurgleId = recvdata['to']
                    self.__is_authenticated = True
                    return gurgle.GURGLE_SUCCESS
                elif 'to' not in recvdata:
                    self.write_log("NO Id returned")
                    self.write_log(recvdata)
                    self.__is_authenticated = False
                    raise gurgle_auth_error('Authenticate Error [No id returned]')
                else:
                    self.write_log("Authenticate Error [%s]"%recvdata['error'])
                    raise gurgle_auth_error('Authenticate Error [%s]'%recvdata['error'])
            else:
                if 'to' not in recvdata:
                    self.write_log("I Have no ID?")
                    self.__is_authenticated = False
                    raise gurgle_auth_error('Authenticate Error [No id returned]')
                self.set_authenticated(True)
                self.__gurgleId = recvdata['to']
                self.__is_authenticated = True
                return gurgle.GURGLE_SUCCESS
        else:
            self.write_log("Server to server authentication is not supported", gurgle.GURGLE_LOG_MODE_ERROR)
            self.__is_authenticated = False
            raise gurgle_auth_error("Server to server authentication is not supported")
    def is_authenticated(self,onlineCheck = False):
        if self.get_runtime_mode() != gurgle.GURGLE_CLIENT:
            return self.__is_authenticated
        else:
            if not onlineCheck:
                return self.__is_authenticated
            message_id = self.create_id()
            senddata = json.dumps({
                    "id"    : message_id,
                    "cmd"   : "query",
                    "obj"   : "auth_status"
                })
            try:
                self.send(encode(senddata))
            except gurgle_network_error as e:
                raise gurgle_network_error(e)
            try:
                recvdata = self.recv(1024,message_id)
            except gurgle_network_error as e:
                raise gurgle_network_error(e)
            if 'params' not in recvdata:
                self.set_authenticated(False)
                return False
            if 'auth_status' not in recvdata['params']:
                self.set_authenticated(False)
                return False
            if recvdata['params']['auth_status'] == 'Authenticated':
                self.set_authenticated(True)
                return True
            else:
                self.set_authenticated(False)
                return False
    def set_authenticated(self,Authenticated):
        self.__is_authenticated = Authenticated
    def ping(self):
        message_id = self.create_id()
        senddata = json.dumps({
                "id"    : message_id,
                "cmd"   : "ping"
            })
        preSentTime = datetime.datetime.now().microsecond;
        if self.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024,message_id)
        if recvdata is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        if 'cmd' not in recvdata:
            self.emergency_quit('SyntaxError','Pong without cmd field')
        postSentTime = datetime.datetime.now().microsecond;
        self.write_log("ping......%.2f ms"%((postSentTime - preSentTime)/1000))
        # do something
        return gurgle.GURGLE_SUCCESS
    def check_auth_method(self,currentUsedMethod):
        message_id = self.create_id()
        data = json.dumps({
                "id"    :message_id,
                "cmd"   :"query",
                "obj"   : "auth_method"
            })
        if self.send(encode(data)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024,message_id)
        if recvdata is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        TempList = None
        if 'params' in recvdata:
            if 'auth_method' in recvdata['params']:
                if list(recvdata['params']['auth_method'])[0] != None:
                    TempList=list(recvdata['params']['auth_method'])
        if TempList == None:
            return False
        self.auth_method_supported  = TempList
        isAuthenticatedMethodSupported = False
        for i in self.auth_method_supported:
            if i == currentUsedMethod:
                isAuthenticatedMethodSupported = True
                break;
        return isAuthenticatedMethodSupported
    def check_version(self):
        message_id = self.create_id()
        data = json.dumps({
                "id"     : message_id,
                "cmd"    : "query",
                "obj"    : "version"
            })
        if self.send(encode(data)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recvdata = self.recv(1024,message_id)
        if recvdata is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        if 'params' not in recvdata:
            return gurgle.GURGLE_FAILED
        if str(recvdata['params']['version']) != self.get_version():
            return gurgle.GURGLE_VERSION_DONOT_MATCH
        return gurgle.GURGLE_SUCCESS
    def get_self_information(self):
        pass
    def is_connected(self):
        return self.__is_connected
    def connect_to_server(self,strDomain,nPort,session=None,timeout=5):
        if self.is_connected():                             #
            self.write_log('You have already connected to remote',gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_ALREADY_CONNECTED
        if strDomain is not None:
            self.set_remote_host(strDomain)
        if nPort is not None:
            self.set_remote_port(nPort)
        if not self.is_remote_addr_set():
            return gurgle.GURGLE_FAILED
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            return gurgle.GURGLE_FAILED
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
        message_id = self.create_id()
        senddata = json.dumps({
            "id"    : message_id,
            "cmd"   : "connect",
            "obj"   : "session",
            "params": {
                "protocol"  : "gurgle",
                "version"   : self.get_version(),
                "encrypt"   : "disabled",
                "session"   : session
            }
        })
        self.send(encode(senddata))
        buf = self.recv(1024,message_id)
        if buf['reply']['status'] == 'connection failed':
            self.write_log("Failed to connect, waiting for details",
                    gurgle.GURGLE_LOG_MODE_ERROR)
            self.recv(1024)
        elif buf['reply']['status'] == 'connection established':
            self.write_log("Connection established",
                    gurgle.GURGLE_LOG_MODE_ERROR)
        else:
            self.write_log("Status %s"%buf['reply']['status'])
        if self.check_version() == gurgle.GURGLE_VERSION_DNOT_MATCH:
            self.write_log("Protocol's version do not match!",
                    gurgle.GURGLE_LOG_MODE_ERROR)
            self.disconnect_from_remote()
            return gurgle.GURGLE_VERSION_DNOT_MATCH
#   Ping
        self.ping()
#   Check Authenticated method
        if self.check_auth_method('plain_password') == False:
            self.write_log("Authenticated method is not supported",
                    gurgle.GURGLE_LOG_MODE_ERROR)
            self.disconnect_from_remote(
                    "Authenticated method is not supported"
                )
            raise gurgle_protocol_error(
                    'Authenticated method is not supported'
                )
        return
    def publish_self_presence_update(self,last_name = None,first_name = None, status = None,mood = None):
        if(last_name == first_name == status == mood == None):
            self.write_log('Empty params')
            return True
        if last_name    != None:
            last_name   = str(last_name)
        if first_name   != None:
            first_name  = str(first_name)
        if status       != None:
            status      = str(status).lower()
            flag        = False
            for i in gurgle.GURGLE_STATUS_SUPPORTED:
                if status == i:
                    flag  = True
                    break
            if flag == False:
                self.write_log('Your status has bad syntax')
                return False
        if mood         != None:
            mood        = str(mood)
        push_id = self.create_id()
        senddata = json.dumps({
                "id"        : push_id,
                "cmd"       : "push",
                "obj"       : "presence",
                "params"    : {
                    "last_name" : last_name,
                    "first_name": first_name,
                    "status"    : status,
                    "mood"      : mood
                }
            })
        self.send(encode(senddata))
        recvdata = self.recv(512,push_id)
        if 'reply' not in recvdata:
            self.write_log( \
                'Recved a bad reply in push presence',
                gurgle.GURGLE_LOG_MODE_ERROR)
            return False
        if recvdata['reply']['error'] != None:
            self.write_log(\
                'Error push presence [%s,%s]'
                    %(recvdata['reply']['error'],recvdata['reply']['reason']),
                gurgle.GURGLE_LOG_MODE_ERROR
                )
            return False
        return True
    def forward_message(self,to,params):
        if to == None or params == None:
            return gurgle.GURGLE_FAILED
        senddata = json.dumps({
            "id"    : 0,
            "cmd"   : "forward",
            "params"    : params
        })
        try:
            self.send(encode(senddata))
        except gurgle_network_error as e:
            raise gurgle_network_error(e)
        return gurgle.GURGLE_SUCCESS
    def new_friend(self,gid,message = None):
        if type(gid) != str:
            return False
        if self.analyse_full_id(gid) == None:
            self.write_log("Bad gid")
            return False
        message_id = self.create_id()
        senddata = {
            "id"    : message_id,
            "cmd"   : "update",
            "obj"   : "roster",
            "params":{
                "gid"           : gid,
                "sub_to"        : True
            }
        }
        if message != None:
            senddata['params']['subscribted_message'] = str(message)
        senddata = json.dumps(senddata)
        self.send(encode(senddata))
        recvdata = self.recv(message_id)
        print ("%s"%recvdata)
    def emergency_quit(self,error="UnknownError",reason="Unkonwn reason",message_id=0):
        cmd = "kill"
        if self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            cmd = "quit"
        if error == None:
            if reason == None:
                senddata = json.dumps({
                "id"    : message_id,
                "cmd"   : cmd
            })
            else:
                senddata = json.dumps({
                "id"    : message_id,
                "cmd"   : cmd,
                "params": {
                    "error"   : error
                }
            })
        else:
            senddata = json.dumps({
                    "id"    : message_id,
                    "cmd"   : cmd,
                    "params": {
                        "error"   : error,
                        "reason"  : reason
                    }
                })
        try:
            self.send(encode(senddata))
        except gurgle_network_error as err:
            pass
        self.__socket.close()
    def reply_error(self,message_id,error,reason,help = None,with_help = False):
        error = str(error)
        reason =  str(reason)
        if with_help == False:
            help = None
        senddata = json.dumps({
                "id"    : message_id,
                "reply" : {
                    "error" : error,
                    "reason": reason,
                    "help"  : help
                }
            })
        if self.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        return gurgle.GURGLE_SUCCESS
    def reply_ok(self,message_id):
        senddata = json.dumps({
                "id"    : message_id,
                "reply" : {
                    "error" : None
                }
            })
        if self.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        return gurgle.GURGLE_SUCCESS
    def disconnect_from_remote(self,                    \
                    reason      = "Unkonwn reason",     \
                    message_id  = 0):
        if not self.is_connected():
            self.write_log('You have not connected to remote!'
                    ,gurgle.GURGLE_LOG_MODE_ERROR)
            return  gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
        elif self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            if message_id == 0:
                message_id = self.create_id()
            senddata = json.dumps({
                "id"    : message_id,
                "cmd"   : "quit",
                "params": {
                    "reason"    : reason
                }
            })
            senddata = senddata.encode()
            self.send(senddata)
            recvdata = self.recv(512,message_id)
            if recvdata == None:
                self.write_log("Connection was closed by peer"
                        ,gurgle.GURGLE_LOG_MODE_ERROR)
                self.__socket.close()
                return gurgle.GURGLE_SUCCESS
            if not 'reply' in recvdata:
                self.write_log('Some errors occupied,Just quit'
                        ,gurgle.GURGLE_LOG_MODE_ERROR)
            else:
                self.write_log('server replied %s'%recvdata['reply'])
        self.__socket.close()
        del self.__socket
        self.__is_connected = False
        self.__is_authenticated = False
        self.__session = None
        return gurgle.GURGLE_SUCCESS


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
        core.plain_password_auth('grgl:%s@%s/%s'
                        %('tricks',
                        '127.0.0.1',
                        core.create_terminal_id()
                        ),'123321123'
                    )
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
    core.publish_self_presence_update(last_name = "SternW",first_name="Zhang",status = "Avaliable")
    core.new_friend("grgl:test@localhost","I want to make friend with you")



