#!/usr/bin/python3
# !encoding='utf-8'
import mysql.connector as mysql
import sys

sys.path.append("%s//..//" % sys.path[0])
from protocol.gurgle import *


class grgl_mysql_controllor_error(Exception):
    pass


class grgl_mysql_controllor:
    DATABASE_HOST = 'localhost'
    DATABASE_USER = 'tricks'
    DATABASE_PASS = '123321123'
    DATABASE_NAME = 'linkc_users'
    USER_INFO_TABLE_NAME = 'user_info'
    DATABASE_PORT = 3306
    DATABASE_SUCCESS = True
    DATABASE_FAILED = False
    ERROR_EMPTY_ARGUMENT = False
    AUTH_FAILED = 0
    AUTH_SUCCESS = 1
    AUTH_INCORRECT = 2

    def __init__(self):
        self.__is_connected = False
        self.__mysql_fd = None
        self.__mysql_conn = None
        self.__log_level = 3
        self.__core = None
        self.alias = None
        self.domain = None
        try:
            self.__mysql_conn = mysql.connect(
                host=self.DATABASE_HOST,
                user=self.DATABASE_USER,
                passwd=self.DATABASE_PASS,
                port=self.DATABASE_PORT
            )
        except mysql.Error as err:
            self.__core.write_log("MySQL Connect Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(err)
        self.__mysql_fd = self.__mysql_conn.cursor()
        self.__is_connected = True
        try:
            self.__mysql_fd.execute(
                "CREATE DATABASE IF NOT EXISTS %s" % self.DATABASE_NAME
            )
        except mysql.Error as err:
            self.__core.write_log("MySQL create database Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_conn = mysql.connect(
                host=self.DATABASE_HOST,
                user=self.DATABASE_USER,
                passwd=self.DATABASE_PASS,
                port=self.DATABASE_PORT,
                db=grgl_mysql_controllor.DATABASE_NAME
            )
        except mysql.Error as err:
            self.__core.write_log("MySQL Connect Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(err)
        self.__mysql_fd = self.__mysql_conn.cursor()
        self.__is_connected = True
        try:
            self.__mysql_fd.execute(
                "create table if not exists %s ("
                "   id int not null primary key auto_increment,"
                "   username char(32) not null,"
                "   password char(32) not null,"
                "   last_name char(16),"
                "   first_name char(16),"
                "   join_time date not null,"
                "   status char(16) not null,"
                "   mood char(255),"
                "   disabled bool not null"
                ")" % self.USER_INFO_TABLE_NAME
            )
        except mysql.Error as err:
            self.__core.write_log("Mysql create table Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(err)

    def is_connected(self):
        return self.__is_connected

    def set_core(self, _core):
        self.__core = _core

    def set_server_alias(self, a, d):
        self.domain = d
        self.alias = a

    def connect_to_database(self, db=None):
        try:
            if db is not None:
                self.__mysql_conn = mysql.connect(
                    host=self.DATABASE_HOST,
                    user=self.DATABASE_USER,
                    passwd=self.DATABASE_PASS,
                    port=self.DATABASE_PORT,
                    database=db
                )
            else:
                self.__mysql_conn = mysql.connect(
                    host=self.DATABASE_HOST,
                    user=self.DATABASE_USER,
                    passwd=self.DATABASE_PASS,
                    port=self.DATABASE_PORT,
                )
        except mysql.Error as err:
            self.__core.write_log("MySQL Connect Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(err)
        self.__mysql_fd = self.__mysql_conn.cursor()
        self.__is_connected = True
        return grgl_mysql_controllor.DATABASE_SUCCESS

    def disconnect_from_database(self):
        """

        :rtype : object
        """
        if self.is_connected():
            if self.__mysql_fd:
                self.__mysql_fd.close()
            if self.__mysql_conn:
                self.__mysql_conn.close()
            self.__is_connected = False
        return grgl_mysql_controllor.DATABASE_SUCCESS

    def plain_password_authenticate(self, username, password, disconnect=True):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        self.__mysql_fd.execute(
            "SELECT disabled FROM %s WHERE username = '%s'"
            % (self.USER_INFO_TABLE_NAME, username))
        data = self.__mysql_fd.fetchone()
        if str(data)[1:-2] is None:
            if disconnect:
                self.disconnect_from_database()
            return grgl_mysql_controllor.AUTH_INCORRECT
        if str(data)[1:-2] == '1':
            if disconnect:
                self.disconnect_from_database()
            return grgl_mysql_controllor.AUTH_FAILED
        self.__mysql_fd.execute(
            "SELECT password FROM %s WHERE username = '%s'"
            % (self.USER_INFO_TABLE_NAME, username))
        db_password = self.__mysql_fd.fetchone()
        self.disconnect_from_database()
        if db_password is None:
            return grgl_mysql_controllor.AUTH_INCORRECT
        if db_password[0] != password:
            return grgl_mysql_controllor.AUTH_INCORRECT
        if disconnect:
            self.disconnect_from_database()
        return grgl_mysql_controllor.AUTH_SUCCESS

    def get_user_presence(self, user, disconnect=True):
        if user is None:
            return None
        try:
            userid = self.get_user_order(user, False)
        except grgl_mysql_controllor_error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        if userid == 0:
            return None
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(
                "SELECT last_name,first_name,status,mood FROM %s WHERE id = '%d'"
                % (self.USER_INFO_TABLE_NAME, userid))
        except mysql.Error as err:
            self.__core.write_log("Database Error : %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        data = self.__mysql_fd.fetchone()
        if disconnect is True:
            self.disconnect_from_database()
        if data is None:
            return None
        (last_name, first_name, status, mood) = data
        return (last_name, first_name, status, mood)

    def get_roster(self, username=None, limit=-1, disconnect=True):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        user_id = self.get_user_order(username, False)
        if user_id == 0:
            return None
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            if limit >= 1:
                self.__mysql_fd.execute(
                    "SELECT id,nickname,groups,sub_from,sub_to FROM subscribed_list_%d WHERE sub_to = 1 LIMIT %d" % (
                        user_id, limit))
            else:
                self.__mysql_fd.execute(
                    "SELECT id,nickname,groups,sub_from,sub_to FROM subscribed_list_%d WHERE sub_to = 1" % user_id)
        except mysql.Error as err:
            raise grgl_mysql_controllor_error(err)
        count = 0
        t_dict = {}
        r_list = []
        tmp_list = []
        while True:
            tmp_var = self.__mysql_fd.fetchone()
            if tmp_var is None:
                break
            count += 1
            t_dict[tmp_var[0]] = (tmp_var[1], tmp_var[2], tmp_var[3], tmp_var[4])
        for i in t_dict.keys():
            tmp_list = []
            try:
                tmp_var = self.get_user_presence(i, disconnect=False)
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error("Cannot fetch user(id=%s)'s presence[%s]" % (i, err))
            if tmp_var is None:
                tmp_list.append(i)
                tmp_list.append("CannotFetchPresence")
                r_list.append(tmp_list)
                continue
            tmp_list.append(i)  # id
            for t in t_dict[i]:
                tmp_list.append(t)
            for k in tmp_var:
                tmp_list.append(k)
            r_list.append(tmp_list)
        self.disconnect_from_database()
        if disconnect:
            self.disconnect_from_database()
        return r_list

    def add_user(self, username=None, password=None, disconnect=True):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        flag = False
        data = 0
        try:
            data = self.get_user_order(username)
        except grgl_mysql_controllor_error as err:
            if str(err) != 'NoSuchUser':
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
            else:
                flag = True
        if data != 0:
            self.__core.write_log("Sign up Error [User already exists]",
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(
                'Failed to add user [User already exists]'
            )
        if password is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(
                "INSERT INTO %s (username,password,join_time,"
                "status,mood,disabled)"
                "VALUES('%s','%s','%s','%s',NULL,0)"
                % (self.USER_INFO_TABLE_NAME, username, password,
                   time.strftime('%Y-%m-%d', time.localtime(time.time())),
                   "invisible"
                )
            )
            self.__mysql_conn.commit()
        except mysql.Error as err:
            self.__core.write_log("Mysql Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(
                "SELECT id FROM %s WHERE username = '%s'"
                % (self.USER_INFO_TABLE_NAME, username))
        except mysql.Error as err:
            self.__core.write_log("Mysql Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(err)
        data = self.__mysql_fd.fetchone()
        try:
            self.__mysql_fd.execute(
                "CREATE TABLE subscribed_list_%d("
                "id CHAR(32) PRIMARY KEY NOT NULL,"
                "nickname CHAR(32),"
                "groups CHAR(32),"
                "sub_to BOOL NOT NULL,"
                "sub_from BOOL NOT NULL"
                ")"
                % data[0])
            self.__mysql_conn.commit()
        except mysql.Error as err:
            self.__core.write_log("Mysql Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(
                "CREATE TABLE offline_message_list_%d (message_id BIGINT,type char(32),time DATETIME, message TEXT)"
                % data[0])
            self.__mysql_conn.commit()
        except mysql.Error as err:
            self.__core.write_log("Mysql Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True

    def update_user_presence(self, username, update_dict, disconnect=True):
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        update_dict = dict(update_dict)
        if update_dict == {}:
            return False
        update_string = ""
        for i in update_dict.keys():
            update_string += i
            update_string += "="
            update_string += '"%s"' % str(update_dict[i])
            update_string += ","
        update_string = str(update_string)[:-1]
        try:
            self.__mysql_fd.execute(
                "UPDATE %s SET %s WHERE username = '%s'"
                % (self.USER_INFO_TABLE_NAME, update_string, username))
            self.__mysql_conn.commit()
        except mysql.Error as err:
            self.__core.write_log("Mysql Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True

    def update_roster_info(self, host_user, target_user, update_dict, disconnect=True):
        if host_user is None or target_user is None:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(host_user) == str:
            host_user = self.get_user_order(host_user, False)
        if type(host_user) != int:
            if disconnect:
                self.disconnect_from_database()
            return False
        if host_user == 0:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(target_user) == str:
            target_user = self.get_user_name(target_user, False)
        if type(target_user) != str:
            if disconnect:
                self.disconnect_from_database()
            return False
        if gurgle.analyse_full_id(gurgle, target_user) is None:
            target_user = gurgle.make_up_full_id(gurgle, target_user, self.domain, None)
        if target_user is None:
            if disconnect:
                self.disconnect_from_database()
            return False
        update_dict = dict(update_dict)
        if update_dict == {}:
            return False
        update_string = ""
        for i in update_dict.keys():
            update_string += i
            update_string += "="
            update_string += '"%s"' % str(update_dict[i])
            update_string += ","
        update_string = str(update_string)[:-1]
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(
                "UPDATE subscribed_list_%d SET %s WHERE id = '%s'"
                % (host_user, update_string, target_user))
            self.__mysql_conn.commit()
        except mysql.Error as err:
            self.__core.write_log("Mysql Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        try:
            self.drop_redundancy_subscribed_info(host_user, False)
        except mysql.Error as err:
            self.__core.write_log("Mysql Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR, self.__log_level)
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True

    def insert_offline_message(self, data, message_id=0, type='message', userid=None,
                               disconnect=True):  # this userid is a str to indentity user on net
        if data is None:
            return False
        if userid is None:
            return False
        try:
            id = self.get_user_order(userid, False)  # this id is an int to indentify user in local database
        except grgl_mysql_controllor_error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        if id == 0:
            if disconnect:
                self.disconnect_from_database()
            return False
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(
                "INSERT INTO offline_message_list_%d (message_id,type,time,message) "
                "VALUES(%d,'%s','%s','%s')"
                % (id, message_id, type, time.strftime('%Y-%m-%d %X', time.localtime(time.time())), data))
            self.__mysql_conn.commit()
        except mysql.Error as err:
            self.__core.write_log("MySQL Error %s" % err,
                                  gurgle.GURGLE_LOG_MODE_ERROR)
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True

    def get_user_name(self, user, disconnect=True):
        if user is None:
            return None
        if type(user) == str:
            tmp_var = gurgle.analyse_full_id(gurgle, user)
            if tmp_var is None:
                username = user
            else:
                username = tmp_var[1]
            if gurgle.is_username_acceptable(gurgle, username) is False:
                return None
            return username
        elif type(user) == int:
            if not self.is_connected():
                try:
                    self.connect_to_database(self.DATABASE_NAME)
                except grgl_mysql_controllor_error as err:
                    self.disconnect_from_database()
                    raise grgl_mysql_controllor_error(err)
            try:
                self.__mysql_fd.execute("SELECT id FROM %s WHERE id = '%d'" % (self.USER_INFO_TABLE_NAME, user))
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
            data = self.__mysql_fd.fetchone()
            if data is None:
                if disconnect:
                    self.disconnect_from_database()
                return 0
            if disconnect:
                self.disconnect_from_database()
            return str(data[0])

    def get_user_order(self, user, disconnect=True):
        if user is None:
            return 0
        if type(user) == int:
            return user
        if type(user) != str:
            return 0
        tmp_var = gurgle.analyse_full_id(gurgle, user)
        if tmp_var is None:
            for ch in user:
                is_found = False
                for a in gurgle.usernameAllowed:
                    if a == ch:
                        is_found = True
                        break
                if is_found is False:
                    return None
            username = user
        else:
            flag = False
            for i in self.alias:
                if tmp_var[2] == i:
                    flag = True
            if flag is False:
                return 0
            username = tmp_var[1]
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute("SELECT id FROM %s WHERE username = '%s'" % (self.USER_INFO_TABLE_NAME, username))
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        data = self.__mysql_fd.fetchone()
        if data is None:
            return 0
        if disconnect:
            self.disconnect_from_database()
        return int(data[0])

    def is_user_subscribed(self, host_user, target_user, target_alias, disconnect=True):
        if host_user is None or target_user is None:
            if disconnect:
                self.disconnect_from_database()
            return -1
        if target_alias is None:
            return -1
        if type(host_user) == str:
            host_user = self.get_user_order(host_user, False)
        if type(host_user) != int:
            if disconnect:
                self.disconnect_from_database()
            return -1
        if host_user == 0:
            if disconnect:
                self.disconnect_from_database()
            return -1
        if type(target_user) == int:
            target_user = self.get_user_name(target_user, False)
        if type(target_user) != str:
            if disconnect:
                self.disconnect_from_database()
            return -1
        if gurgle.analyse_full_id(gurgle, target_user) is None:
            target_user = gurgle.make_up_full_id(gurgle, target_user, self.domain, None)
        if target_user is None:
            if disconnect:
                self.disconnect_from_database()
            return -1
        user_id_a = target_user
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute("SELECT id,sub_to FROM subscribed_list_%d" % host_user)
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        tmp_varA = gurgle.analyse_full_id(gurgle, user_id_a)
        while (1):
            user_id_b = self.__mysql_fd.fetchone()
            if user_id_b is None:
                break
            tmp_var_b = gurgle.analyse_full_id(gurgle, user_id_b[0])
            if tmp_var_b is None:
                return -1
            flag = False
            if tmp_varA[1] == tmp_var_b[1]:
                for i in target_alias:
                    if tmp_var_b[2] == i:
                        self.__mysql_fd.fetchmany()
                        if disconnect:
                            self.disconnect_from_database()
                        if user_id_b[1] == 1:
                            return 1
                        else:
                            return 0
        if disconnect:
            self.disconnect_from_database()
        return -1

    def delete_offline_message(self, userid=None, message_id=0, disconnect=True):
        if userid is None:
            return False
        if type(userid) == str:
            tmp_var = gurgle.analyse_full_id(gurgle, userid)
            if tmp_var is None:
                return False
            try:
                userid = self.get_user_order(tmp_var[1], False)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
            flag = False
            for i in self.alias:
                if i == tmp_var[2]:
                    flag = True
            if flag is False:
                if disconnect:
                    self.disconnect_from_database()
                return False
        if userid == 0:
            return False
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            if message_id > 0:
                self.__mysql_fd.execute("DELETE FROM offline_message_%d WHERE message_id='%d'" % (userid, message_id))
            else:
                self.__mysql_fd.execute("DELETE FROM offline_message_%d" % (userid))
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True

    def get_offline_message(self, userid=None, message_type='message', message_id=0, delete=True, limit=100,
                            disconnect=True):
        if userid is None:
            return None
        if type(userid) == str:
            tmp_var = gurgle.analyse_full_id(gurgle, userid)
            if tmp_var is None:
                return None
            flag = False
            for i in self.alias:
                if i == tmp_var[2]:
                    flag = True
            if flag is False:
                return None
            try:
                userid = self.get_user_order(tmp_var[1], False)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        if userid == 0:
            return None
        if limit == 0:
            return None
        if limit < 0 and limit != -1:
            return None
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            if message_id == 0:
                if message_type is not None:
                    self.__mysql_fd.execute(
                        "SELECT message_id,message FROM offline_message_list_%d WHERE type='%s' LIMIT %d"
                        % (userid, message_type, limit))
                else:
                    self.__mysql_fd.execute("SELECT message_id,message FROM offline_message_list_%d LIMIT %d"
                                            % (userid, limit))
            else:
                if message_type is not None:
                    self.__mysql_fd.execute(
                        "SELECT message_id,message FROM offline_message_list_%d WHERE type='%s' and message_id='%d' LIMIT %d"
                        % (userid, message_type, message_id, limit))
                else:
                    self.__mysql_fd.execute(
                        "SELECT message_id,message FROM offline_message_list_%d WHERE message_id='%d' LIMIT %d"
                        % (userid, message_id, limit))
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        ret_data = []
        id_list = []
        while True:
            data = self.__mysql_fd.fetchone()
            if data is None:
                break
            (msg_id, msg) = data
            ret_data.append(msg)
            id_list.append(msg_id)
        if delete:
            for i in id_list:
                self.__mysql_fd.execute("DELETE FROM offline_message_list_%d WHERE message_id='%d'" % (userid, i))
            try:
                self.__mysql_conn.commit()
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return (id_list, ret_data)

    def subscribe(self, host_user, target_user, target_user_alias,
                  disconnect=True):  # from[who send request] to[who will accept request]
        if host_user is None or target_user is None:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(host_user) == str:
            host_user = self.get_user_order(host_user, False)
        if type(host_user) != int:
            if disconnect:
                self.disconnect_from_database()
            return False
        if host_user == 0:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(target_user) == int:
            target_user = self.get_user_name(target_user, False)
        if type(target_user) != str:
            if disconnect:
                self.disconnect_from_database()
            return False
        if gurgle.analyse_full_id(gurgle, target_user) is None:
            target_user = gurgle.make_up_full_id(gurgle, target_user, self.domain, None)
        if target_user is None:
            if disconnect:
                self.disconnect_from_database()
            return False
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            status = self.is_user_subscribed(host_user, target_user, target_user_alias, False)
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        if type(target_user) == int:
            return False  ## !!!!!!!!!!!!!!!!!!!!!!
        tmpA = gurgle.analyse_full_id(gurgle, target_user)
        target_user = gurgle.make_up_full_id(gurgle, tmpA[1], target_user_alias[0], None)
        if status == -1:
            try:
                self.__mysql_fd.execute(
                    "INSERT INTO subscribed_list_%d (id,sub_to,sub_from) VALUE ('%s',true,false)" % (
                        host_user, target_user))
                self.__mysql_conn.commit()
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        elif status == 0:
            try:
                print ("Update")
                self.__mysql_fd.execute(
                    "UPDATE subscribed_list_%d SET sub_to = 1 WHERE id='%s'" % (host_user, target_user))
                self.__mysql_conn.commit()
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True

    def unsubscribe(self, host_user, target_user, disconnect=False):
        if host_user is None or target_user is None:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(host_user) == str:
            host_user = self.get_user_order(host_user, False)
        if type(host_user) != int:
            if disconnect:
                self.disconnect_from_database()
            return False
        if host_user == 0:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(target_user) == int:
            target_user = self.get_user_name(target_user, False)
        if type(target_user) != str:
            if disconnect:
                self.disconnect_from_database()
            return False
        if gurgle.analyse_full_id(gurgle, target_user) is None:
            target_user = gurgle.make_up_full_id(gurgle, target_user, self.domain, None)
        if target_user is None:
            if disconnect:
                self.disconnect_from_database()
            return False
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(
                "SELECT sub_from FROM subscribed_list_%d WHERE id = '%s'" % (host_user, target_user))
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        tmp_var = self.__mysql_fd.fetchone()
        if tmp_var is None:
            return True
        if bool(tmp_var) is False:
            try:
                self.__mysql_fd.execute("DELETE FROM subscribed_list_%d WHERE id='%s'" % (host_user, target_user))
                self.__mysql_conn.commit()
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        else:
            try:
                self.__mysql_fd.execute(
                    "UPDATE subscribed_list_%d SET sub_to = false WHERE id='%s'" % (host_user, target_user))
                self.__mysql_conn.commit()
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.drop_redundancy_subscribed_info(host_user, False)
        except grgl_mysql_controllor_error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True

    def drop_redundancy_subscribed_info(self, target_user, disconnect=True):
        if target_user is None:
            return
        if type(target_user) == str:
            target_user = self.get_user_order(target_user, False)
        if type(target_user) != int:
            if disconnect:
                self.disconnect_from_database()
            return False
        if target_user == 0:
            if disconnect:
                self.disconnect_from_database()
            return False
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute("DELETE FROM subscribed_list_%d WHERE sub_from = 0 and sub_to = 0" % target_user)
            self.__mysql_conn.commit()
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return


if __name__ == '__main__':
    try:
        s = grgl_mysql_controllor()
    except grgl_mysql_controllor_error:
        sys.exit(0)
    while True:
        username = input("Username: ")
        password = input("Password: ")
        s.add_user(username, password)
        if input("Continue?(y/n)").lower() != 'y':
            break
