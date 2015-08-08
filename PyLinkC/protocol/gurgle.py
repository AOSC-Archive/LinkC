#!/usr/bin/python3
# !encoding='utf-8'

import socket
import json
import _thread
import threading  # for mutex
import random
import sys
from codecs import decode, encode
import string
import time
import datetime


class packageNode:
    nextNode = None
    data = None
    ext = None
    package_id = 0

    def __init__(self, node_data):
        self.data = node_data


class packageList:
    def __init__(self):
        self.size = 0
        self.root = packageNode(None)
        self.root = None
        return

    def __init__(self):
        self.root = None
        self.size = 0

    def __del__(self):
        if self.root is None:
            return
        self.root = None
        self.size = 0

    def insert(self, new_data, package_id, package_ext=None):
        new_node = packageNode(new_data)
        new_node.package_id = package_id
        new_node.ext = package_ext
        if self.root is None:
            self.root = new_node
            self.size += 1
            return
        temp_node = self.root
        while temp_node.nextNode is not None:
            temp_node = temp_node.nextNode
        temp_node.nextNode = new_node
        self.size += 1

    def get_data(self, package_id, package_ext=None, server_alias=[]):
        if self.size == 0:
            return None
        else:
            temp_node = self.root
            for i in range(0, self.size):
                if temp_node.package_id == package_id:
                    if package_id == 0:
                        if gurgle.is_id_match(gurgle, package_ext, temp_node.ext, server_alias):
                            return temp_node.data
                    else:
                        return temp_node.data
                temp_node = temp_node.nextNode
            return None

    def remove(self, package_id, package_ext=None):
        if self.root is None:
            return False
        if self.root.package_id == package_id:
            if package_id != 0:
                temp_node = self.root.nextNode
                self.root = None
                self.size -= 1
                self.root = temp_node
                return True
            else:
                if package_ext is not None:
                    if self.root.ext == package_ext:
                        temp_node = self.root.nextNode
                        self.root = None
                        self.size -= 1
                        self.root = temp_node
                        return True
                    else:
                        pass
                else:
                    temp_node = self.root.nextNode
                    self.root = None
                    self.size -= 1
                    self.root = temp_node
                    return True
        cur_node = self.root
        while cur_node.nextNode is not None:
            if cur_node.nextNode.package_id == package_id:
                cur_node.nextNode = cur_node.nextNode.nextNode
                self.size -= 1
                return True
            elif package_id == 0:
                if package_ext is not None:
                    if cur_node.nextNode.ext == package_ext:
                        cur_node.nextNode = cur_node.nextNode.nextNode
                        self.size -= 1
                        return True
                else:
                    cur_node.nextNode = cur_node.nextNode.nextNode
                    self.size -= 1
                    return True
            cur_node = cur_node.nextNode
        return False

    def get_root(self):
        return self.root

    def get_size(self):
        return self.size

    def print_list(self):
        temp_node = self.root
        while temp_node is not None:
            print (temp_node.data)
            temp_node = temp_node.nextNode


class door_lock():
    def __init__(self):
        self.__lock = threading.Lock()

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
    auth_method_supported = ['plain_password']
    GURGLE_LOG_MODE_ERROR = 1
    GURGLE_LOG_MODE_COMMON = 2
    GURGLE_LOG_MODE_DEBUG = 3
    GURGLE_CLIENT = 1
    GURGLE_SERVER = 2
    GURGLE_GROUP = 3
    GURGLE_FAILED = False
    GURGLE_SUCCESS = True
    GURGLE_HOST_NOT_SET = 10
    GURGLE_PORT_NOT_SET = 11
    GURGLE_ALREADY_CONNECTED = 12
    GURGLE_HAVENT_CONNECTED = 13
    GURGLE_FAILED_TO_AUTH = 14
    GURGLE_FAILED_TO_CREATE_SOCKET = 15
    GURGLE_FAILED_TO_CONNECT_TO_REMOTE = 16
    GURGLE_FAILED_TO_RECV = 17
    GURGLE_FAILED_TO_SEND = 18
    GURGLE_CONNECTION_CLOSED = 19
    GURGLE_VERSION_DNOT_MATCH = 20
    GURGLE_STATUS_SUPPORTED = ['available', 'away', 'dnd', 'invisible']
    passwordAllowed = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                       'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                       'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                       '!', '@', '#', '$', '%', '^', '&', '*', '-', '_', '=', '<', '>',
                       '+', '?', '/', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0']

    usernameAllowed = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                       'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                       'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                       '+', '-', '_', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0']

    def __init__(self, _mode):
        self.__gurgleVersion = 'Unusable'
        self.__remoteHost = None
        self.__remotePort = None
        self.__is_connected = False
        self.__runtime_mode = _mode
        self.__socket = None
        self.__package_list = packageList()
        self.__auth_method = ['plain_password']
        self.__terminal_id = None
        self.__is_authenticated = False
        self.__roster = None
        self.__roster_etag = None
        self.__log_level = 3
        self.__send_mutex = threading.Lock()
        self.__recv_mutex = threading.Lock()
        self.__recv_door_1 = door_lock()
        self.__recv_door_2 = door_lock()
        self.__recv_roster = 0
        self.__recv_door_2.door_close()
        self.__gurgleId = None
        self.__session = None
        if self.__runtime_mode == gurgle.GURGLE_CLIENT:
            self.write_log('Gurgle version %s %s'
                           % (self.__gurgleVersion, 'initialized as Client'))
        if self.__runtime_mode == gurgle.GURGLE_SERVER:
            self.write_log('Gurgle version %s %s'
                           % (self.__gurgleVersion, 'initialized as Server'))
        if self.__runtime_mode == gurgle.GURGLE_GROUP:
            self.write_log('Gurgle version %s %s'
                           % (self.__gurgleVersion, 'initialized as Group server'))

    def __del__(self):
        self.write_log('Gurgle Deleting....')
        if self.is_connected():
            self.disconnect_from_remote("Program was terminated")
        del self.__recv_door_1
        del self.__recv_door_2
        del self.__recv_mutex
        del self.__send_mutex
        del self.__package_list

    def get_version(self):
        return self.__gurgleVersion

    def set_socket(self, sockfd_):
        self.__socket = sockfd_
        self.__is_connected = True

    def create_id(self):
        return random.randint(0, 2147483647)

    def create_random_str(self, length):
        return ''.join(random.sample(
            string.ascii_letters + string.digits, length))

    def create_terminal_id(self):
        if not self.__terminal_id:
            return ''.join(random.sample(string.ascii_letters + string.digits, 8))
        else:
            return self.__terminal_id

    def write_log(self, log, mode=None, l_level=None):
        if mode is None:
            mode = self.GURGLE_LOG_MODE_COMMON
        if l_level is not None:
            level = int(l_level)
        else:
            level = self.get_log_level()
        if mode > level:
            return None
        if mode == gurgle.GURGLE_LOG_MODE_ERROR:  # error mode
            sys.stderr.write("[%s] : %s\n"
                             % (time.asctime(time.localtime()), log))
        elif mode == gurgle.GURGLE_LOG_MODE_COMMON:  # common mode
            sys.stdout.write("[%s] : %s\n"
                             % (time.asctime(time.localtime()), log))
        elif mode == gurgle.GURGLE_LOG_MODE_DEBUG:
            sys.stdout.write("[%s] : %s\n"
                             % (time.asctime(time.localtime()), log))

    def __recv_lock_release(self):
        self.__recv_mutex.release()
        self.__recv_door_1.door_close()
        self.__recv_door_2.door_open()
        while self.__recv_roster:
            self.__recv_door_2.door_step_into()
        self.__recv_door_2.door_close()
        self.__recv_door_1.door_open()

    def recv(self, buf_size=512, message_id=0, message_obj=None, timeout=5, max_try=2):
        if self.is_connected() is False:
            return None
        id_ = 0
        n_try = 0
        while n_try < max_try:
            self.__recv_door_1.door_rush_into()
            if self.__recv_mutex.acquire(blocking=False):
                break
            self.__recv_roster += 1
            self.__recv_door_2.door_step_into()
            data = self.__package_list.get_data(message_id)
            self.__recv_roster -= 1
            self.__recv_door_2.door_step_out()
            if data is not None:
                self.__package_list.remove(message_id, message_obj)
                return data
            if timeout == 0:
                return None
            n_try += 1
            # status :: door_1 closed, door_2 opened, recv_mutex locked
        if not self.is_connected():
            self.__recv_lock_release()
            raise gurgle_network_error("Connection has not been established!")
        self.__socket.settimeout(timeout)
        return_data = None
        obj = None
        buf = None
        n_try = 0
        while n_try < max_try:
            data = self.__package_list.get_data(0)
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
                n_try += 1
                self.write_log("Timeout")
                continue
            except socket.error as e:
                self.write_log("Error %s"%e)
                self.write_log(e, gurgle.GURGLE_LOG_MODE_ERROR)
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
            buf = buf.decode()
            start = 0
            location = 0
            count = 0
            z_buf = []
            return_buf = None
            for ch in buf:
                if ch == "{":
                    if count == 0:
                        start = location
                    count += 1
                if ch == "}":
                    count -= 1
                    if count == 0:
                        z_buf.append(buf[start:location + 1])
                location += 1
            for buf in z_buf:
                try:
                    buf = json.loads(buf)
                except ValueError as e:
                    self.write_log(e, gurgle.GURGLE_LOG_MODE_ERROR)
                    self.write_log("Error package = [%s]" % buf)
                    n_try += 1
                    continue
                if 'id' not in buf:
                    if message_id == 0:
                        return_buf = buf
                        n_try += 1
                        continue
                else:
                    id_ = int(buf['id'])
                if 'cmd' in buf:
                    if buf['cmd'] == 'kill':
                        error = None
                        reason = None
                        if 'params' in buf:
                            if 'error' in buf['params']:
                                error = str(buf['params']['error'])
                            if 'reason' in buf['params']:
                                reason = str(buf['params']['reason'])
                        self.write_log("Connection was closed [%s, %s]"
                                       % (error, reason),
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
                    if return_buf is not None:
                        self.__package_list.insert(buf, id)
                        n_try += 1
                        continue
                    return_buf = buf
                    n_try += 1
                    continue
                if id_ == message_id:
                    if return_buf is not None:
                        self.write_log("WHAT HAPPENED?")
                    return_buf = buf
                    n_try += 1
                    continue
                else:
                    self.__package_list.insert(buf, id)
            if return_buf is not None:
                self.__recv_lock_release()
                return return_buf
            n_try += 1
        self.__recv_lock_release()
        return None

    def send(self, buf):
        if self.is_connected() is False:
            raise gurgle_network_error('Connection has not been established')
        self.__send_mutex.acquire()
        try:
            self.__socket.send(buf)
        except socket.error as e:
            self.write_log(e, gurgle.GURGLE_LOG_MODE_ERROR)
            self.__send_mutex.release()
            raise gurgle_network_error(e)
        self.__send_mutex.release()
        return gurgle.GURGLE_SUCCESS

    def set_remote_host(self, str_host):
        self.__remoteHost = str_host

    def set_remote_port(self, n_port):
        self.__remotePort = n_port

    def get_runtime_mode(self):
        return self.__runtime_mode

    def get_remote_host(self):
        return self.__remoteHost

    def get_remote_port(self):
        return self.__remotePort

    def get_auth_method(self):
        return self.__auth_method

    def get_roster_e_tag(self):
        return self.__roster_etag

    def set_roster(self):
        pass

    def set_log_level(self, level):
        self.__log_level = int(level)

    def get_session(self):
        return self.__session

    def get_log_level(self):
        return self.__log_level

    def get_self_gurgle_id(self):
        if self.__is_authenticated:
            return self.__gurgleId
        return None

    def is_username_acceptable(self, username):
        if username is None:
            return False
        if type(username) != str:
            return False
        for ch in username:
            is_found = False
            for a in self.usernameAllowed:
                if a == ch:
                    is_found = True
                    break
            if is_found is False:
                return False
        return True

    def is_password_acceptable(self, password):
        if password is None:
            return False
        if type(password) != str:
            return False
        for ch in password:
            is_found = False
            for a in self.usernameAllowed:
                if a == ch:
                    is_found = True
                    break
            if is_found is False:
                return False
        return True

    def analyse_full_id(self, full_id):
        if full_id is None:
            return None
        try:
            (protocol, ID) = full_id.split(':', 1)
        except ValueError as err:
            return None
        if ID.find("@") == -1:
            return None
        (username, suffix) = ID.split("@", 1)
        if (username is None) or (suffix is None):
            return None
        if suffix.find("/") == -1:
            return (protocol, username, suffix, None)
        else:
            (domain, terminal) = suffix.split("/", 1)
            if domain is None:
                return None
            return (protocol, username, domain, terminal)

    def make_up_full_id(self, user_name, domain, terminal=None):
        if (user_name is None) or (domain is None):
            return 'SyntaxError'
        if terminal is None:
            return "grgl:%s@%s" % (user_name, domain)
        if self.is_username_acceptable(user_name) is False:
            return None
        return "grgl:%s@%s/%s" % (user_name, domain, terminal)

    def is_id_match(self, user_id_a, user_id_b, server_alias):
        if user_id_a is None or user_id_b is None:
            return False
        tmp_var_a = self.analyse_full_id(user_id_a)
        if tmp_var_a is None:
            return False
        tmp_var_b = self.analyse_full_id(user_id_b)
        if tmp_var_b is None:
            return False
        if tmp_var_a[1] != tmp_var_b[1]:
            return False
        flag_a = False
        flag_b = False
        for i in server_alias:
            if i == tmp_var_b[2]:
                flag_a = True
            if i == tmp_var_a[2]:
                flag_b = True
        if flag_a and flag_b:
            return True
        return False

    def is_remote_addr_set(self):
        if not self.__remoteHost:
            self.write_log('Remote addr is not set!'
                           , gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_HOST_NOT_SET
        if not self.__remotePort:
            self.write_log('Remote port is not set!'
                           , gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_PORT_NOT_SET
        return gurgle.GURGLE_SUCCESS

    def request_roster(self):
        # check whether authenticated
        pass

    def query_roster_update(self, user_id_1, user_id_2, domain, ip):
        pass

    def query_server_alias(self):
        if self.is_connected() is False:
            return None
        return None

    def push_roster_update(self, params):
        senddata = {
            "id": self.create_id(),
            "cmd": "update",
            "obj": "roster",
            "params": params
        }
        try:
            self.send(encode(json.dumps(senddata)))
        except ValueError as err:
            raise ValueError(err)
        except gurgle_network_error as err:
            raise gurgle_auth_error(err)
        return

    def plain_password_auth(self, id_, password, protocol='grgl'):
        try:
            status = self.is_authenticated()
        except gurgle_network_error as e:
            raise gurgle_network_error(e)
        if status is True:
            self.write_log("You have already been authenticated")
            return gurgle.GURGLE_SUCCESS
        if not (id_ and password):
            self.write_log('Username or password is empty!', gurgle.GURGLE_LOG_MODE_ERROR)
            raise gurgle_auth_error('Username or password is empty')
        if self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            message_id = self.create_id()
            senddata = json.dumps({
                "id": message_id,
                "cmd": "auth",
                "from": id_,
                "params": {
                    "method": self.get_auth_method(),
                    "password": password
                }
            })
            self.send(encode(senddata))
            try:
                recv_data = self.recv(512, message_id)
            except gurgle_network_error as e:
                raise gurgle_network_error(e)
            if 'reply' not in recv_data:
                self.write_log("NO REPLY")
                self.__is_authenticated = False
                return gurgle.GURGLE_FAILED
            if 'error' in recv_data['reply']:
                if recv_data['reply']['error'] is None:
                    self.set_authenticated(True)
                    self.__gurgleId = recv_data['to']
                    self.__is_authenticated = True
                    return gurgle.GURGLE_SUCCESS
                elif 'to' not in recv_data:
                    self.write_log("NO Id returned")
                    self.write_log(recv_data)
                    self.__is_authenticated = False
                    raise gurgle_auth_error('Authenticate Error [No id_ returned]')
                else:
                    self.write_log("Authenticate Error [%s]" % recv_data['error'])
                    raise gurgle_auth_error('Authenticate Error [%s]' % recv_data['error'])
            else:
                if 'to' not in recv_data:
                    self.write_log("I Have no ID?")
                    self.__is_authenticated = False
                    raise gurgle_auth_error('Authenticate Error [No id returned]')
                self.set_authenticated(True)
                self.__gurgleId = recv_data['to']
                self.__is_authenticated = True
                return gurgle.GURGLE_SUCCESS
        else:
            self.write_log("Server to server authentication is not supported", gurgle.GURGLE_LOG_MODE_ERROR)
            self.__is_authenticated = False
            raise gurgle_auth_error("Server to server authentication is not supported")

    def is_authenticated(self, online_check=False):
        if self.get_runtime_mode() != gurgle.GURGLE_CLIENT:
            return self.__is_authenticated
        else:
            if not online_check:
                return self.__is_authenticated
            message_id = self.create_id()
            senddata = json.dumps({
                "id": message_id,
                "cmd": "query",
                "obj": "auth_status"
            })
            try:
                self.send(encode(senddata))
            except gurgle_network_error as e:
                raise gurgle_network_error(e)
            try:
                recv_data = self.recv(1024, message_id)
            except gurgle_network_error as e:
                raise gurgle_network_error(e)
            if 'reply' not in recv_data:
                self.set_authenticated(False)
                return False
            if 'auth_status' not in recv_data['reply']:
                self.set_authenticated(False)
                return False
            if recv_data['reply']['auth_status'] == 'Authenticated':
                self.set_authenticated(True)
                return True
            else:
                self.set_authenticated(False)
                return False

    def set_authenticated(self, authenticated):
        self.__is_authenticated = authenticated

    def ping(self):
        message_id = self.create_id()
        senddata = json.dumps({
            "id": message_id,
            "cmd": "ping"
        })
        pre_sent_time = datetime.datetime.now().microsecond
        if self.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recv_data = self.recv(1024, message_id)
        if recv_data is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        if 'cmd' not in recv_data:
            self.emergency_quit('SyntaxError', 'Pong without cmd field')
        post_sent_time = datetime.datetime.now().microsecond
        self.write_log("ping......%.2f ms" % ((post_sent_time - pre_sent_time) / 1000))
        # do something
        return gurgle.GURGLE_SUCCESS

    def check_auth_method(self, current_used_method):
        message_id = self.create_id()
        data = json.dumps({
            "id": message_id,
            "cmd": "query",
            "obj": "auth_method"
        })
        if self.send(encode(data)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recv_data = self.recv(1024, message_id)
        if recv_data is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        temp_list = None
        if 'reply' in recv_data:
            if 'auth_method' in recv_data['reply']:
                if recv_data['reply']['auth_method'][0] is not None:
                    temp_list = list(recv_data['reply']['auth_method'])
        if temp_list is None:
            return False
        self.auth_method_supported = temp_list
        flag = False
        for i in self.auth_method_supported:
            if i == current_used_method:
                flag = True
                break
        return flag

    @property
    def check_version(self):
        message_id = self.create_id()
        data = json.dumps({
            "id": message_id,
            "cmd": "query",
            "obj": "version"
        })
        if self.send(encode(data)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        recv_data = self.recv(1024, message_id)
        if recv_data is None:
            return gurgle.GURGLE_FAILED_TO_RECV
        if 'reply' not in recv_data:
            return gurgle.GURGLE_FAILED
        if str(recv_data['reply']['version']) != self.get_version():
            return False
        return True

    def get_self_information(self):
        pass

    def is_connected(self):
        return self.__is_connected

    def connect_to_server(self, str_domain, n_port, session=None, timeout=5):
        if self.is_connected():  #
            self.write_log('You have already connected to remote', gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_ALREADY_CONNECTED
        if str_domain is not None:
            self.set_remote_host(str_domain)
        if n_port is not None:
            self.set_remote_port(n_port)
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
                           , gurgle.GURGLE_LOG_MODE_ERROR)
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
            "id": message_id,
            "cmd": "connect",
            "obj": "session",
            "params": {
                "protocol": "gurgle",
                "version": self.get_version(),
                "encrypt": "disabled",
                "session": session
            }
        })
        self.send(encode(senddata))
        buf = self.recv(1024, message_id)
        if buf['reply']['status'] == 'connection failed':
            self.write_log("Failed to connect, waiting for details",
                           gurgle.GURGLE_LOG_MODE_ERROR)
            self.recv(1024)
        elif buf['reply']['status'] == 'connection established':
            self.write_log("Connection established",
                           gurgle.GURGLE_LOG_MODE_ERROR)
        else:
            self.write_log("Status %s" % buf['reply']['status'])
        if self.check_version == gurgle.GURGLE_VERSION_DNOT_MATCH:
            self.write_log("Protocol's version do not match!",
                           gurgle.GURGLE_LOG_MODE_ERROR)
            self.disconnect_from_remote()
            return gurgle.GURGLE_VERSION_DNOT_MATCH
        #   Ping
        self.ping()
        #   Check Authenticated method
        if self.check_auth_method('plain_password') is False:
            self.write_log("Authenticated method is not supported",
                           gurgle.GURGLE_LOG_MODE_ERROR)
            self.disconnect_from_remote(
                "Authenticated method is not supported"
            )
            raise gurgle_protocol_error(
                'Authenticated method is not supported'
            )
        return

    def publish_self_presence_update(self, last_name=None, first_name=None, status=None, mood=None):
        if (last_name == first_name == status == mood is None):
            self.write_log('Empty params')
            return True
        if last_name is not None:
            last_name = str(last_name)
        if first_name is not None:
            first_name = str(first_name)
        if status is not None:
            status = str(status).lower()
            flag = False
            for i in gurgle.GURGLE_STATUS_SUPPORTED:
                if status == i:
                    flag = True
                    break
            if flag is False:
                self.write_log('Your status has bad syntax')
                return False
        if mood is not None:
            mood = str(mood)
        push_id = self.create_id()
        senddata = json.dumps({
            "id": push_id,
            "cmd": "push",
            "obj": "presence",
            "params": {
                "last_name": last_name,
                "first_name": first_name,
                "status": status,
                "mood": mood
            }
        })
        self.send(encode(senddata))
        recv_data = self.recv(512, push_id)
        if 'reply' not in recv_data:
            self.write_log('Recved a bad reply in push presence', gurgle.GURGLE_LOG_MODE_ERROR)
            return False
        if recv_data['reply']['error'] is not None:
            self.write_log(
                'Error push presence [%s,%s]'
                % (recv_data['reply']['error'], recv_data['reply']['reason']),
                gurgle.GURGLE_LOG_MODE_ERROR
            )
            return False
        return True

    def forward_message(self, to, params):
        if to is None or params is None:
            return gurgle.GURGLE_FAILED
        senddata = json.dumps({
            "id": 0,
            "cmd": "forward",
            "params": params
        })
        try:
            self.send(encode(senddata))
        except gurgle_network_error as e:
            raise gurgle_network_error(e)
        return gurgle.GURGLE_SUCCESS

    def new_friend(self, gid, message=None):
        if type(gid) != str:
            return False
        if self.analyse_full_id(gid) is None:
            self.write_log("Bad gid")
            return False
        message_id = self.create_id()
        senddata = {
            "id": message_id,
            "cmd": "update",
            "obj": "roster",
            "params": {
                "gid": gid,
                "sub_to": True
            }
        }
        if message is not None:
            senddata['params']['subscribed_message'] = str(message)
        senddata = json.dumps(senddata)
        self.send(encode(senddata))
        recv_data = self.recv(message_id)
        print ("%s" % recv_data)

    def emergency_quit(self, error="UnknownError", reason="Unkonwn reason", message_id=0):
        cmd = "kill"
        if self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            cmd = "quit"
        if error is None:
            if reason is None:
                senddata = json.dumps({
                    "id": message_id,
                    "cmd": cmd
                })
            else:
                senddata = json.dumps({
                    "id": message_id,
                    "cmd": cmd,
                    "params": {
                        "error": error
                    }
                })
        else:
            senddata = json.dumps({
                "id": message_id,
                "cmd": cmd,
                "params": {
                    "error": error,
                    "reason": reason
                }
            })
        try:
            self.send(encode(senddata))
        except gurgle_network_error as err:
            pass
        self.__socket.close()

    def reply_error(self, message_id, error, reason, help=None, with_help=False):
        error = str(error)
        reason = str(reason)
        if with_help is False:
            help = None
        senddata = json.dumps({
            "id": message_id,
            "reply": {
                "error": error,
                "reason": reason,
                "help": help
            }
        })
        if self.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        return gurgle.GURGLE_SUCCESS

    def reply_ok(self, message_id):
        senddata = json.dumps({
            "id": message_id,
            "reply": {
                "error": None
            }
        })
        if self.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
            return gurgle.GURGLE_FAILED_TO_SEND
        return gurgle.GURGLE_SUCCESS

    def disconnect_from_remote(self, reason="Unknown reason", message_id=0):
        if not self.is_connected():
            self.write_log('You have not connected to remote!'
                           , gurgle.GURGLE_LOG_MODE_ERROR)
            return gurgle.GURGLE_FAILED_TO_CONNECT_TO_REMOTE
        elif self.get_runtime_mode() == gurgle.GURGLE_CLIENT:
            if message_id == 0:
                message_id = self.create_id()
            senddata = json.dumps({
                "id": message_id,
                "cmd": "quit",
                "params": {
                    "reason": reason
                }
            })
            senddata = senddata.encode()
            self.send(senddata)
            recv_data = self.recv(512, message_id)
            if recv_data is None:
                self.write_log("Connection was closed by peer"
                               , gurgle.GURGLE_LOG_MODE_ERROR)
                self.__socket.close()
                return gurgle.GURGLE_SUCCESS
            if not 'reply' in recv_data:
                self.write_log('Some errors occupied,Just quit'
                               , gurgle.GURGLE_LOG_MODE_ERROR)
            else:
                self.write_log('server replied %s' % recv_data['reply'])
        self.__socket.close()
        del self.__socket
        self.__is_connected = False
        self.__is_authenticated = False
        self.__session = None
        return gurgle.GURGLE_SUCCESS


if __name__ == '__main__':
    core = gurgle(gurgle.GURGLE_CLIENT)
    try:
        core.connect_to_server('127.0.0.1', 40097)
    except gurgle_network_error:
        pass
    except gurgle_protocol_error:
        pass
    #   Auth
    try:
        core.plain_password_auth('grgl:%s@%s/%s'
                                 % ('tricks',
                                    '127.0.0.1',
                                    core.create_terminal_id()
        ), '123321123'
        )
    except gurgle_auth_error as err:
        #   Failed to authenticate
        core.write_log("Auth failed")
        del core
        exit()
    except gurgle_network_error as e:
        core.write_log("Network error:%s" % e)
        del core
        exit()
    core.write_log("Auth succeed")
    core.publish_self_presence_update(last_name="SternW", first_name="Zhang", status="Avaliable")
    core.new_friend("grgl:test@localhost", "I want to make friend with you")



