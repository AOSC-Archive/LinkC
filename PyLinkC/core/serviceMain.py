#!/usr/bin/python3
# !encoding='utf-8'

import sys
import _thread

sys.path.append("%s//..//" % sys.path[0])
from protocol.gurgle import *
from database.grgl_mysql import *
from codecs import decode, encode


protocolSupported = ['grgl']


class ServiceThread(threading.Thread):
    core            = None
    grgl_mysql      = None
    addr            = None
    is_authenticated = "Unauthenticated"
    senddata        = "something to send"
    username        = None
    terminal        = None
    session         = None
    FullSignInID    = None
    userid          = 0
    alias = ['127.0.0.1', 'localhost', '192.168.1.1', '117.59.15.96']
    global_domain = 'localhost'

    def setup(self, _socket, _addr):
        self.core = gurgle(gurgle.GURGLE_SERVER)
        self.grgl_mysql = grgl_mysql_controllor()
        self.addr = _addr
        self.core.set_socket(_socket)
        self.core.set_remote_host(self.addr[0])
        self.core.set_remote_port(self.addr[1])
        self.setName("TemporaryConnection")
        self.grgl_mysql.set_server_alias(self.alias, self.global_domain)
        self.grgl_mysql.set_core(self.core)

    def __del__(self):
        del self.core
        del self.grgl_mysql

    def get_user_id(self):
        pass

    def grgl_forward_message(self, name, data, domain=None, terminal=None):
        if domain is None:
            domain = self.global_domain
        flag = False
        for i in self.alias:
            if i == domain:
                flag = True
        if flag is True:
            if terminal is None:
                for i in threading.enumerate():
                    t_name = i.getName()
                    if t_name == "MainThread":
                        continue
                    elif t_name == "TemporaryConnection":
                        continue
                    else:
                        if i.username == name:
                            try:
                                i.core.send(data)
                            except gurgle_network_error :
                                raise gurgle_network_error(None)
                            return True
            else:
                for i in threading.enumerate():
                    t_name = i.getName()
                    if t_name == "MainThread":
                        continue
                    elif t_name == "TemporaryConnection":
                        continue
                    else:
                        if i.username == name and i.terminal == terminal:
                            i.core.send(data)
                            return True
        return False

    def run(self):
        """
            type self: object
        """
        params = {}
        while True:
            senddata    = None
            try:
                data = self.core.recv(1024)
            except gurgle_network_error:
                _thread.exit()
            if data == None:
                continue
            if 'id' not in data:
                self.core.write_log("Data without ID", gurgle.GURGLE_LOG_MODE_ERROR)
                self.core.emergency_quit('SyntaxError', 'This package has no ID!')
                _thread.exit()
            if type(data['id']) != int:
                self.core.write_log("Bad ID", gurgle.GURGLE_LOG_MODE_ERROR)
                self.core.emergency_quit('SyntaxError', 'Bad id')
                _thread.exit()
            message_id = int(data['id'])
            if 'cmd' not in data:
                self.core.reply_error(message_id, "SyntaxError", "No CMD")
                continue
            if type(data['cmd']) != str:
                self.core.reply_error(message_id, "SyntaxError", "Bad cmd")
                continue
            cmd = data['cmd']
            if 'obj' not in data:
                obj = None
            else:
                if data['obj'] is None:
                    obj = None
                elif type(data['obj']) == str:
                    obj = data['obj']
                else:
                    self.core.reply_error(message_id, "SyntaxError", "Bad obj")
                    continue
            if 'params' in data:
                if data['params'] is None:
                    params = None
                elif type(data['params']) != dict:
                    self.core.reply_error(message_id, "SyntaxError", "Bad params")
                    continue
                else:
                    params = data['params']
            if cmd == 'ping':  # ping
                if 'payload' in data:
                    senddata = json.dumps({
                        "id": message_id,
                        "cmd": "pong",
                        "payload": data['payload']
                    })
                else:
                    senddata = json.dumps({
                        "id": message_id,
                        "cmd": "pong"
                    })
            elif cmd == 'connect':
                if obj == 'session':
                    if params is None:
                        self.core.reply_error(message_id, "SyntaxError", "No params")
                        continue
                    if 'protocol' not in params:
                        senddata = json.dumps({
                            "id": message_id,
                            "reply": {
                                "status": "connection failed"
                            }
                        })
                        self.core.send(encode(senddata))
                        self.core.write_log(
                            'Connection does not specify protocol'
                            , gurgle.GURGLE_LOG_MODE_ERROR)
                        self.core.emergency_quit(
                            'ProtocolError',
                            'Connection does not specify protocol'
                        )
                        _thread.exit()
                    else:  # there is protocol specified in params
                        if str(params['protocol']) != 'gurgle':
                            senddata = json.dumps({
                                "id": message_id,
                                "reply": {
                                    "status": "connection failed"
                                }
                            })
                            self.core.send(encode(senddata))
                            self.core.write_log('Protocol %s is not supported'
                                                % params['protocol']
                                                , gurgle.GURGLE_LOG_MODE_ERROR)
                            self.core.emergency_quit(
                                'ProtocolError',
                                'Protocol is not supported'
                            )
                            _thread.exit()

                            # gurgle protocol connection's start
                    if 'version' in params:
                        if str(params['version']) != self.core.get_version():
                            senddata = json.dumps({
                                "id": message_id,
                                "reply": {
                                    "status": "connection failed"
                                }
                            })
                            self.core.send(encode(senddata))
                            self.core.write_log(
                                "Protocol's version[%s] does not match"
                                % str(params['version'])
                                , gurgle.GURGLE_LOG_MODE_ERROR)
                            self.core.emergency_quit(
                                'ProtocolError',
                                "Protocol's version[%s] does not match"
                                % str(params['version'])
                            )
                            _thread.exit()
                    if 'session' in params:
                        tmp_session = str(params['session'])[0:8]
                    else:
                        tmp_session = self.core.create_terminal_id()
                    for i in threading.enumerate():
                        t_name = i.getName()
                        if t_name == "MainThread":
                            continue
                        if i.session == tmp_session:
                            self.core.emergency_quit("SessionConflicted", "Session conflicted, plz try again")
                            _thread.exit()
                    self.session = tmp_session
                    senddata = json.dumps({
                        "id": message_id,
                        "reply": {
                            "status": "connection established",
                            "session": self.session
                        }
                    })
            elif cmd == 'query':  # Query
                if obj == 'auth_method':
                    senddata = json.dumps({
                        "id": message_id,
                        "reply": {
                            "auth_method": self.core.get_auth_method()
                        }
                    })
                elif obj == 'auth_status':
                    senddata = json.dumps({
                        "id": message_id,
                        "reply": {
                            "auth_status": self.is_authenticated
                        }
                    })
                elif obj == 'version':
                    senddata = json.dumps({
                        "id": message_id,
                        "reply": {
                            "version": self.core.get_version()
                        }
                    })
                elif obj == 'server_alias':
                    senddata = json.dumps({
                        "id": message_id,
                        "reply": {
                            "count": len(self.alias),
                            "value": self.alias
                        }
                    })
                elif obj == 'presence':
                    presence_status = None
                    mood            = None
                    last_name_      = None
                    first_name_     = None
                    if 'who' not in params:
                        if self.is_authenticated != "Authenticated":
                            self.core.reply_error(message_id, "SyntaxError", "Whose presence you want to query?")
                            continue
                        try:
                            tmp_var = self.grgl_mysql.get_user_presence(self.username)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id, 'DatabaseError', err)
                            continue
                        if tmp_var is not None:
                            (last_name_, first_name_, presence_status, mood) = tmp_var
                        else:
                            self.core.reply_error(message_id, "DatabaseError", "Cannot fetch your presence")
                            continue
                    else:
                        tmp_var = self.core.analyse_full_id(params['who'])
                        if tmp_var is None:
                            self.core.reply_error(message_id, "SyntaxError",
                                                  "Bad gid in params:{'who':'%s'}" % params['who'])
                            continue
                        try:
                            tmp_var = self.grgl_mysql.get_user_presence(tmp_var[1])
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id, 'DatabaseError', err)
                            continue
                        if tmp_var is not None:
                            (last_name_, first_name_, presence_status, mood) = tmp_var
                    senddata = json.dumps({
                        "id": message_id,
                        "reply": {
                            "last_name" : last_name_,
                            "first_name": first_name_,
                            "status": presence_status,
                            "mood": mood,
                            "error": None
                        }
                    })
                elif obj == 'roster':
                    if self.is_authenticated is False:
                        self.core.reply_error(message_id, "PermissionDenied", "Unauthenticated")
                        continue
                    limit = 100
                    if params is not None:
                        if 'limit' in params:
                            limit = int(params['limit'])
                    try:
                        tmp_var = self.grgl_mysql.get_roster(self.username, limit=limit)
                    except grgl_mysql_controllor_error as err:
                        self.core.reply_error(message_id, "DatabaseError", err)
                        continue
                    senddata = json.dumps({
                        "id": message_id,
                        "reply": {
                            "value": tmp_var
                        }
                    })
                else:  # end if of [query]
                    self.core.emergency_quit(
                        'UnknownObj',
                        "Obj[%s] isn't supported"
                        % obj,
                        message_id
                    )
                    self.core.write_log("Querying obj[%s] isn't supported"
                                        % obj
                                        , gurgle.GURGLE_LOG_MODE_ERROR)
                    _thread.exit()
            elif cmd == 'auth':
                if 'from' not in data:
                    self.core.write_log("Auth without from",
                                        gurgle.GURGLE_LOG_MODE_ERROR)
                    self.core.reply_error(message_id, 'SyntaxError', 'No from field')
                    continue
                self.FullSignInID = data['from']
                if self.FullSignInID.find(':') == -1:
                    self.core.reply_error(message_id, 'SyntaxError', "No ':' found in ID")
                    continue
                (protocol, i_d) = self.FullSignInID.split(':', 1)
                if protocol == 'grgl':
                    tmp_data = self.core.analyse_full_id(self.FullSignInID)
                    if tmp_data is None:
                        self.core.reply_error(message_id, 'SyntaxError', 'ID cannot be analysed')
                        continue
                    (protocol, self.username, domain, self.terminal) = tmp_data
                    flag = False
                    for i in self.alias:
                        if i == domain:
                            flag = True
                    if flag is False:
                        self.core.reply_error(message_id, "Unsupported", "Unsupported")
                        continue
                    if self.terminal is None:
                        self.terminal = self.core.create_terminal_id()
                    if self.core.is_username_acceptable(self.username) is False:
                        self.core.reply_error(message_id, 'SyntaxError', 'Disallowed character was founded')
                        continue
                    if params is None:
                        self.core.reply_error(message_id, 'SyntaxError', 'Auth without the params field')
                        continue
                    if 'password' not in params:
                        self.core.reply_error(message_id, 'SyntaxError', 'No password')
                        continue
                    if type(params['password']) != str:
                        self.core.reply_error(message_id, 'SyntaxError', 'Bad password')
                        continue
                    pass_word = params['password']
                    if pass_word == '':
                        self.core.reply_error(message_id, 'SyntaxError', 'No password')
                        continue
                    if self.core.is_password_acceptable(pass_word) is False:
                        self.core.reply_error(message_id, 'SyntaxError', 'Username or password incorrect')
                        continue
                    result = self.grgl_mysql.plain_password_authenticate(self.username, pass_word)
                    self.FullSignInID = self.core.make_up_full_id(self.username, self.alias[0], self.terminal)
                    if result == grgl_mysql_controllor.AUTH_SUCCESS:
                        try:
                            self.userid = self.grgl_mysql.get_user_order(self.username)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id, "DatabaseError", "Cannot get your ID")
                            continue
                        senddata = json.dumps({
                            "id": message_id,
                            "to": self.FullSignInID,
                            "reply": {
                                "error": None
                            }
                        })
                        self.is_authenticated = 'Authenticated'
                        self.setName(self.FullSignInID)
                        try:
                            self.core.send(encode(senddata))
                        except gurgle_network_error as err:
                            self.core.write_log(err, gurgle.GURGLE_LOG_MODE_ERROR)
                            self.core.emergency_quit()
                            _thread.exit()
                        try:
                            tmp_var = self.grgl_mysql.get_offline_message(self.FullSignInID)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id, "DatabaseError", "Cannot fetch offline message[%s]"
                                                  % err)
                            continue
                        if tmp_var is None:
                            continue
                        (id_list, data_list) = tmp_var
                        i = 0
                        while i < len(data_list):
                            try:
                                self.core.send(encode(data_list[i]))
                            except gurgle_network_error as err:
                                self.core.emergency_quit()
                                _thread.exit()
                            i += 1
                        try:
                            (id_list, data_list) = self.grgl_mysql.get_offline_message(self.FullSignInID,
                                                                                       message_type=None, delete=True)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id, "DatabaseError", "Cannot fetch offline message[%s]"
                                                  % err)
                            continue
                        i = 0
                        while i < len(data_list):
                            try:
                                self.core.send(encode(data_list[i]))
                            except gurgle_network_error as err:
                                self.core.emergency_quit()
                                _thread.exit()
                            i += 1
                        continue
                    elif result == grgl_mysql_controllor.AUTH_INCORRECT:
                        self.is_authenticated = 'Unauthenticated'
                        self.core.reply_error(message_id, 'AuthFailed', 'Username or password is incorrect')
                        continue
                    else:
                        self.core.reply_error(message_id, 'AuthFailed', 'Your account has been disabled or deactivated')
                        self.is_authenticated = 'Unauthenticated'
                        continue
                else:
                    self.core.write_log("Protocol[%s] is not supported yet"
                                        % protocol, gurgle.GURGLE_LOG_MODE_ERROR)
                    self.core.emergency_quit(
                        "ProtocolUnSupported",
                        "Protocol[%s] is not supported yet"
                        % protocol,
                        message_id
                    )
                    self.core.reply_error(message_id, 'ProtocolUnSupported',
                                          "Protocol[%s] is not supported yet" % protocol)
                    self.core.emergency_quit(None, None, 0)
                    _thread.exit()
            elif cmd == 'push':
                if self.is_authenticated == "Unauthenticated":
                    self.core.reply_error(message_id, "PermissionDenied", "Unauthenticated")
                    continue
                if params is None:
                    self.core.reply_error(message_id, "SyntaxError", "Push without params")
                    continue
                if obj == 'presence':
                    update_dict = {}
                    if 'last_name' in params:
                        update_dict['last_name'] = str(params['last_name'])
                    if 'first_name' in params:
                        update_dict['first_name'] = str(params['first_name'])
                    if 'status' in params:
                        if params['status'] is not None:
                            for i in self.core.GURGLE_STATUS_SUPPORTED:
                                if params['status'].lower() == i:
                                    update_dict['status'] = i
                                    break
                    if 'mood' in params:
                        update_dict['mood'] = str(params['mood'])
                    if update_dict == {}:
                        self.core.reply_error(message_id, "SyntaxError", "Please specify what you want to modify")
                        continue
                    try:
                        self.grgl_mysql.update_user_presence(self.username, update_dict)
                    except grgl_mysql_controllor_error as err:
                        self.core.reply_error(message_id, "UnknownError", err)
                        continue
                    self.core.reply_ok(message_id)
                    continue
            elif cmd == 'update':
                if obj is None:
                    self.core.reply_error(message_id, "SyntaxError", "Tell what you want to update use 'obj' field")
                    continue
                if params is None:
                    self.core.reply_error(message_id, "SyntaxError",
                                          "Describe waht you want to update use 'params' field")
                    continue
                if type(params) != dict:
                    self.core.reply_error(message_id, "SyntaxError", "Bad params")
                    continue
                if obj == 'roster':
                    if 'gid' not in params:
                        self.core.reply_error(message_id, "SyntaxError", "Bad params[No gid specified]")
                        continue
                    target_user = params['gid']
                    if self.core.analyse_full_id(target_user) is None:
                        target_user = self.core.make_up_full_id(target_user, self.global_domain)
                    if target_user is None:
                        self.core.reply_error(message_id, "SyntaxError", "Bad GID in params")
                        continue
                    update_dict = {}
                    if 'nickname' in params:
                        update_dict['nickname'] = str(params['nickname'])
                    if 'groups' in params:
                        update_dict['groups'] = str(params['groups'])
                    flag = False
                    if 'sub_to' in params:
                        flag = True
                        try:
                            subscribed_status = self.grgl_mysql.is_user_subscribed(self.userid, target_user, self.alias)
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id, "DatabaseError", err)
                            continue
                        if bool(params['sub_to']) is True:
                            if subscribed_status < 1:
                                try:
                                    presence_status = self.grgl_mysql.subscribe(self.userid, target_user, self.alias)
                                except grgl_mysql_controllor_error as err:
                                    self.core.reply_error(message_id, "DatabaseError", err)
                                    continue
                                if presence_status is False:
                                    self.core.reply_error(message_id, "UnknownError", "Cannot subscribe him/her")
                                    continue
                                senddata = {
                                    "id": self.core.create_id(),
                                    "cmd": "push",
                                    "obj": "subscribed_message",
                                    "params": {
                                        "message": "%s just subscribed you" % self.FullSignInID,
                                        "subscribed_status": True
                                    }
                                }
                                if 'subscribed_message' in params:
                                    senddata['params']['message'] = params['subscribed_message']
                                tmp_var = self.core.analyse_full_id(target_user)
                                if tmp_var is None:
                                    self.core.reply_error(message_id, "UnknownError", None)
                                    continue
                                senddata = json.dumps(senddata)
                                try:
                                    presence_status = self.grgl_forward_message(tmp_var[1], encode(senddata), tmp_var[2]
                                                                                , tmp_var[3])
                                except gurgle_network_error as err:
                                    self.core.reply_error(message_id, "NetworkError", err)
                                    continue
                                if presence_status is False:
                                    try:
                                        self.grgl_mysql.insert_offline_message(senddata, 0, 'subscribed_message',
                                                                               target_user)
                                    except grgl_mysql_controllor_error as err:
                                        self.core.reply_error(message_id, "DatabaseError", err)
                                        continue
                        else:
                            if subscribed_status == 1:
                                try:
                                    self.grgl_mysql.unsubscribe(self.userid, target_user)
                                except grgl_mysql_controllor_error as err:
                                    self.core.reply_error(message_id, "DatabaseError", err)
                                    continue
                                senddata = {
                                    "id": self.core.create_id(),
                                    "cmd": "push",
                                    "obj": "subscribed_message",
                                    "params": {
                                        "message": "%s just unsubscribed you" % self.FullSignInID,
                                        "subscribed_status": False
                                    }
                                }
                                if 'subscribed_message' in params:
                                    senddata['params']['message'] = params['subscribed_message']
                                tmp_var = self.core.analyse_full_id(target_user)
                                if tmp_var is None:
                                    self.core.reply_error(message_id, "UnknownError", None)
                                    continue
                                senddata = json.dumps(senddata)
                                try:
                                    presence_status = self.grgl_forward_message(tmp_var[1], encode(senddata)
                                                                                , tmp_var[2], tmp_var[3])
                                except gurgle_network_error as err:
                                    self.core.reply_error(message_id, "NetworkError", err)
                                    continue
                                if presence_status is False:
                                    try:
                                        self.grgl_mysql.insert_offline_message(senddata, 0, 'subscribed_message'
                                                                               , target_user)
                                    except grgl_mysql_controllor_error as err:
                                        self.core.reply_error(message_id, "DatabaseError", err)
                                        continue
                    if update_dict == {}:
                        if flag:
                            self.core.reply_ok(message_id)
                            continue
                        self.core.reply_error(message_id, "SyntaxError", "Please specify what you want to modify")
                        continue
                    try:
                        self.grgl_mysql.update_roster_info(self.get_user_id(), target_user, update_dict)
                    except grgl_mysql_controllor_error as err:
                        self.core.reply_error(message_id, "DatabaseError", err)
                        continue
                    self.core.reply_ok(message_id)
                    continue
            elif cmd == 'forward':  # forward messages
                if self.is_authenticated == "Unauthenticated":
                    self.core.reply_error(message_id, "PermissionDenied", "Unauthenticated")
                    continue
                if params is None:
                    self.core.reply_error(message_id, "SyntaxError",
                                          "Use 'params' field to tell us what you want to forward")
                    continue
                if 'to' not in params:
                    self.core.reply_error(message_id, "SyntaxError", "Forwarding without to field")
                tmp_var = self.core.analyse_full_id(params['to'])
                if tmp_var is None:
                    self.core.reply_error(message_id, "SyntaxError", "Address cannot be analysed")
                    continue
                if obj != 'message':
                    pass  # something other to send
                if 'message' not in params:
                    self.core.reply_error(message_id, "SyntaxError", "Forward without message",
                                          "Please use 'message' field to tell us what you want to forward", True)
                    continue
                try:
                    senddata = json.dumps({
                        "id": message_id,
                        "cmd": "push",
                        "obj": "message",
                        "params": {
                            "from": self.FullSignInID,
                            "message": params['message']
                        }
                    })
                except (TypeError, ValueError):
                    self.core.reply_error(message_id, "ValueError", "Cannot forward such message")
                    continue
                if tmp_var[0] == 'grgl':
                    try:
                        presence_status = self.grgl_forward_message(tmp_var[1], encode(senddata), tmp_var[2]
                                                                    , tmp_var[3])
                    except gurgle_network_error:
                        self.core.reply_error(message_id, "NetworkError", "Cannot forward message")
                        continue
                    if presence_status is False:
                        try:
                            presence_status = self.grgl_mysql.insert_offline_message(senddata, message_id
                                                                                     , userid=tmp_var[1])
                        except grgl_mysql_controllor_error as err:
                            self.core.reply_error(message_id, "DatabaseError",
                                                  "Failed to insert offline message[%s]" % err)
                            self.core.write_log("DatabaseError[%s]" % err, gurgle.GURGLE_LOG_MODE_ERROR)
                            continue
                    if presence_status is False:
                        self.core.reply_error(message_id, "FailedToFroward"
                                              , "Failed to forward message(Is user unreachable?)")
                        continue
                else:
                    self.core.reply_error(message_id, "ProtocolUnSupported",
                                          "Protocol[%s] is not supported yet" % tmp_var[0])
                    continue
                self.core.reply_ok(message_id)
                continue
            elif cmd == 'quit':
                if 'params' in data:
                    if 'reason' in params:
                        self.core.write_log('Client quited because %s'
                                            % str(params['reason']),
                                            gurgle.GURGLE_LOG_MODE_DEBUG)
                else:
                    self.core.write_log('Client quited without reason',
                                        gurgle.GURGLE_LOG_MODE_DEBUG)
                senddata = json.dumps({"id": message_id, "reply": "bye"})
                self.core.send(encode(senddata))
                self.core.disconnect_from_remote()
                _thread.exit()
            else:  # end if of [cmd]
                self.core.write_log("Such cmd[%s] isn't supported" % data['cmd']
                                    , gurgle.GURGLE_LOG_MODE_ERROR)
                self.core.emergency_quit(
                    'UnknownCmd',
                    "Cmd[%s] isn't supported" % data['cmd'],
                    message_id
                )
                _thread.exit()
            if self.core.send(encode(senddata)) != gurgle.GURGLE_SUCCESS:
                self.core.emergency_quit()
                _thread.exit()
        _thread.exit()


if __name__ == '__main__':
    SERVICE_PORT = 40097
    ADDR = ('', SERVICE_PORT)
    sevSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sevSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sevSocket.bind(ADDR)
    sevSocket.listen(5)
    while True:
        clientSocket, addr = sevSocket.accept()
        newThread = ServiceThread(None, None, None, (), {})
        newThread.setup(clientSocket, addr)
        newThread.start()
