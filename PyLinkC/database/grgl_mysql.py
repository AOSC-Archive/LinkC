#!/usr/bin/python3
#!encoding='utf-8'
import mysql.connector as mysql
import sys
sys.path.append("%s//..//"%sys.path[0])
from protocol.gurgle import *

class grgl_mysql_controllor_error(Exception):
    pass

class grgl_mysql_controllor:
    DATABASE_HOST       = 'localhost'
    DATABASE_USER       = 'root'
    DATABASE_PASS       = '123321123'
    DATABASE_NAME       = 'linkc_users'
    USER_INFO_TABLE_NAME= 'user_info'
    DATABASE_PORT       = 3306
    DATABASE_SUCCESS    = True
    DATABASE_FAILED     = False
    ERROR_EMPRY_ARGUMENT= False
    AUTH_FAILED         = 0
    AUTH_SUCCESS        = 1
    AUTH_INCORRECT      = 2
    def __init__(self):
        self.__is_connected     = False
        self.__mysql_fd         = None
        self.__mysql_conn       = None
        self.__log_level        = 3
        self.global_domain      = None
        self.global_ip          = None
        try:
            self.__mysql_conn   = mysql.connect(    \
                    host = self.DATABASE_HOST,      \
                    user = self.DATABASE_USER,      \
                    passwd = self.DATABASE_PASS,    \
                    port = self.DATABASE_PORT
                )
        except mysql.Error as err:
            gurgle.write_log(gurgle,"MySQL Connect Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        self.__mysql_fd         = self.__mysql_conn.cursor()
        self.__is_connected     = True
        try:
            self.__mysql_fd.execute(    \
                    "CREATE DATABASE IF NOT EXISTS %s"%self.DATABASE_NAME
                )
        except mysql.Error as err:
            gurgle.write_log(gurgle,"MySQL create database Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_conn   = mysql.connect(    \
                    host = self.DATABASE_HOST,      \
                    user = self.DATABASE_USER,      \
                    passwd = self.DATABASE_PASS,    \
                    port = self.DATABASE_PORT,      \
                    db = grgl_mysql_controllor.DATABASE_NAME
                )
        except mysql.Error as err:
            gurgle.write_log(gurgle,"MySQL Connect Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        self.__mysql_fd         = self.__mysql_conn.cursor()
        self.__is_connected     = True
        try:
            self.__mysql_fd.execute(    \
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
                    ")"%self.USER_INFO_TABLE_NAME
                )
        except mysql.Error as err:
            gurgle.write_log(gurgle,"Mysql create table Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
    def is_connected(self):
        return self.__is_connected
    def set_global_definition(self,domain,ip):
        self.global_domain  = domain
        self.global_ip      = ip
    def connect_to_database(self,db = None):
        try:
            if db != None:
                self.__mysql_conn   = mysql.connect(    \
                        host = self.DATABASE_HOST,      \
                        user = self.DATABASE_USER,      \
                        passwd = self.DATABASE_PASS,    \
                        port = self.DATABASE_PORT,      \
                        database = db                   \
                    )
            else:
                self.__mysql_conn   = mysql.connect(    \
                        host = self.DATABASE_HOST,      \
                        user = self.DATABASE_USER,      \
                        passwd = self.DATABASE_PASS,    \
                        port = self.DATABASE_PORT,      \
                    )
        except mysql.Error as err:
            gurgle.write_log(gurgle,"MySQL Connect Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        self.__mysql_fd         = self.__mysql_conn.cursor()
        self.__is_connected     = True
        return grgl_mysql_controllor.DATABASE_SUCCESS
    def disconnect_from_database(self):
        if self.is_connected():
            if self.__mysql_fd:
                self.__mysql_fd.close()
            if self.__mysql_conn:
                self.__mysql_conn.close()
            self.__is_connected = False
        return grgl_mysql_controllor.DATABASE_SUCCESS
    def plain_password_authenticate(self,username,password,disconnect=True):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        self.__mysql_fd.execute(    \
                "SELECT disabled FROM %s WHERE username = '%s'"
                %(self.USER_INFO_TABLE_NAME,username))
        data = self.__mysql_fd.fetchone()
        if str(data)[1:-2] == None:
            if disconnect:
                self.disconnect_from_database()
            return grgl_mysql_controllor.AUTH_INCORRECT
        if str(data)[1:-2] == '1':
            if disconnect:
                self.disconnect_from_database()
            return grgl_mysql_controllor.AUTH_FAILED
        self.__mysql_fd.execute(    \
                "SELECT password FROM %s WHERE username = '%s'"
                %(self.USER_INFO_TABLE_NAME,username))
        db_password = self.__mysql_fd.fetchone()
        self.disconnect_from_database()
        if db_password == None:
            return grgl_mysql_controllor.AUTH_INCORRECT
        if db_password[0] != password:
            return grgl_mysql_controllor.AUTH_INCORRECT
        if disconnect:
            self.disconnect_from_database()
        return grgl_mysql_controllor.AUTH_SUCCESS
    def get_user_presence(self,user,disconnect=True):
        if user == None:
            return None
        try:
            userid = self.get_user_order(user,False)
        except grgl_mysql_controllor_error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        if userid == 0:
            if disconnect:
                self.disconnect_from_database()
            return None
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(    \
                "SELECT last_name,first_name,status,mood FROM %s WHERE id = '%d'"
                %(self.USER_INFO_TABLE_NAME,userid))
        except mysql.Error as err:
            gurgle.write_log(gurgle,"Database Error : %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        data = self.__mysql_fd.fetchone()
        if disconnect == True:
            self.disconnect_from_database()
        if data is None:
            return None
        (last_name,first_name,status,mood) = data
        return (last_name,first_name,status,mood)
    def get_roster(self,username = None,limit=-1,disconnect=True):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        user_id = self.get_user_order(username,False)
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
                self.__mysql_fd.execute("SELECT id,nickname,groups,sub_from,sub_to FROM subscribed_list_%d LIMIT %d"%(user_id,limit))
            else:
                self.__mysql_fd.execute("SELECT id,nickname,groups,sub_from,sub_to FROM subscribe_list_%d"%user_id)
        except mysql.Error as err:
            raise grgl_mysql_controllor_error(err)
        count = 0
        t_dict = {}
        r_list = []
        tmp_list = []
        while True:
            tmpVar = self.__mysql_fd.fetchone()
            if tmpVar == None:
                break
            count += 1
            t_dict[tmpVar[0]] = (tmpVar[1],tmpVar[2],tmpVar[3],tmpVar[4])
        for i in t_dict.keys():
            tmp_list = []
            try:
                tmpVar = self.get_user_presence(i,disconnect=False)
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error("Cannot fetch user(id=%d)'s presence[%s]"%(i,err))
            if tmpVar == None:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error("Cannot fetch user(id=%d)'s presence"%i)
            tmp_list.append(i);         # id
            for t in t_dict[i]:
                tmp_list.append(t);
            for k in tmpVar:
                tmp_list.append(k)
            r_list.append(tmp_list)
        self.disconnect_from_database()
        if disconnect:
            self.disconnect_from_database()
        return r_list
    def add_user(self,username = None, password = None,disconnect=True):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        flag = False
        try:
            data  = self.get_user_order(username)
        except grgl_mysql_controllor_error as err:
            if str(err) != 'NoSuchUser':
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
            else:
                flag = True
        if flag == False:
            gurgle.write_log(gurgle,"Sign up Error [User already exists]",
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
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
            self.__mysql_fd.execute(    \
                    "INSERT INTO %s (username,password,join_time,"
                    "status,mood,disabled)"
                    "VALUES('%s','%s','%s','%s',NULL,0)"
                    %(self.USER_INFO_TABLE_NAME,username,password,\
                        time.strftime('%Y-%m-%d',time.localtime(time.time())),
                        "invisible"
                    )
                )
            self.__mysql_conn.commit();
        except mysql.Error as err:
            gurgle.write_log(gurgle,"Mysql Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(    \
                    "SELECT id FROM %s WHERE username = '%s'"
                    %(self.USER_INFO_TABLE_NAME,username))
        except mysql.Error as err:
            gurgle.write_log(gurgle,"Mysql Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        data = self.__mysql_fd.fetchone()
        try:
            self.__mysql_fd.execute(    \
                    "CREATE TABLE subscribed_list_%d("
                        "id CHAR(32) PRIMARY KEY NOT NULL,"
                        "nickname CHAR(32),"
                        "groups CHAR(32),"
                        "sub_to BOOL NOT NULL,"
                        "sub_from BOOL NOT NULL"
                    ")"
                    %data[0])
            self.__mysql_conn.commit();
        except mysql.Error as err:
            gurgle.write_log(gurgle,"Mysql Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(    \
                    "CREATE TABLE offline_message_list_%d (message_id BIGINT,type char(32),time DATETIME, message TEXT)"
                    %data[0])
            self.__mysql_conn.commit();
        except mysql.Error as err:
            gurgle.write_log(gurgle,"Mysql Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True
    def update_user_presence(self,username,update_dict,disconnect=True):
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
        for i in  update_dict.keys():
            update_string += i
            update_string += "="
            update_string += '"%s"'%str(update_dict[i])
            update_string += ","
        update_string = str(update_string)[:-1]
        try:
            self.__mysql_fd.execute(    \
                    "UPDATE %s SET %s WHERE username = '%s'"
                    %(self.USER_INFO_TABLE_NAME,update_string,username))
            self.__mysql_conn.commit();
        except mysql.Error as err:
            gurgle.write_log(gurgle,"Mysql Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True
    def update_roster_info(self,host_user,target_user,update_dict,disconnect=True):
        if host_user == None or target_user == None:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(host_user) == str:
            host_user = self.get_user_order(host_user,False)
        if type(host_user) != int:
            if disconnect:
                self.disconnect_from_database()
            return False
        if host_user == 0:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(target_user) == str:
            target_user = self.get_user_name(target_user,False)
        if type(target_user) != str:
            if disconnect:
                self.disconnect_from_database()
            return False
        if gurgle.analyse_full_id(gurgle,target_user) == None:
            target_user = gurgle.make_up_full_id(gurgle,target_user,self.global_domain,None)
        if target_user == None:
            if disconnect:
                self.disconnect_from_database()
            return False
        update_dict = dict(update_dict)
        if update_dict == {}:
            return False
        update_string = ""
        for i in  update_dict.keys():
            update_string += i
            update_string += "="
            update_string += '"%s"'%str(update_dict[i])
            update_string += ","
        update_string = str(update_string)[:-1]
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(    \
                    "UPDATE subscribed_list_%d SET %s WHERE id = '%s'"
                    %(host_user,update_string,target_user))
            self.__mysql_conn.commit();
        except mysql.Error as err:
            gurgle.write_log(gurgle,"Mysql Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True
    def insert_offline_message(self,data,message_id=0,type='message',userid=None,disconnect=True):  # this userid is a str to indentity user on net
        if data == None:
            return False
        if userid == None:
            return False
        try:
            id = self.get_user_order(userid,False)   # this id is an int to indentify user in local database
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
            self.__mysql_fd.execute(    \
                "INSERT INTO offline_message_list_%d (message_id,type,time,message) "
                "VALUES(%d,'%s','%s','%s')"
                %(id,message_id,type,time.strftime('%Y-%m-%d %X',time.localtime(time.time())),data))
            self.__mysql_conn.commit();
        except mysql.Error as err:
            gurgle.write_log(gurgle,"MySQL Error %s"%err,
                             gurgle.GURGLE_LOG_MODE_ERROR)
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True
    def get_user_name(self,user,disconnect=True):
        if user == None:
            return None
        if type(user) == str:
            tmpVar = gurgle.analyse_full_id(gurgle,user)
            if tmpVar == None:
                username = user
            else:
                username = tmpVar[1]
            if gurgle.is_username_acceptable(gurgle,username) == False:
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
                self.__mysql_fd.execute("SELECT id FROM %s WHERE id = '%d'"%(self.USER_INFO_TABLE_NAME,user))
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
            data = self.__mysql_fd.fetchone()
            if data == None:
                if disconnect:
                    self.disconnect_from_database()
                return 0
            if disconnect:
                self.disconnect_from_database()
            return str(data[0])
    def get_user_order(self,user,disconnect=True):
        if user == None:
            return 0
        if type(user) == int:
            return user
        if type(user) != str:
            return 0
        tmpVar = gurgle.analyse_full_id(gurgle,user)
        if tmpVar == None:
            for ch in user:
                isFound = False
                for a in gurgle.usernameAllowed:
                    if a == ch:
                        isFound = True
                        break
                if isFound == False:
                    return None
            username = user
        else:
            if tmpVar[2] != self.global_domain and tmpVar[2] != self.global_ip:
                return 0
            username = tmpVar[1]
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute("SELECT id FROM %s WHERE username = '%s'"%(self.USER_INFO_TABLE_NAME,username))
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        data = self.__mysql_fd.fetchone()
        if data == None:
            if disconnect:
                self.disconnect_from_database()
            raise grgl_mysql_controllor_error("NoSuchUser")
        if disconnect:
            self.disconnect_from_database()
        return int(data[0])
    def is_user_subscribed(self,host_user,target_user,disconnect=True):
        if host_user == None or target_user == None:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(host_user) == str:
            host_user = self.get_user_order(host_user,False)
        if type(host_user) != int:
            if disconnect:
                self.disconnect_from_database()
            return False
        if host_user == 0:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(target_user) == int:
            target_user = self.get_user_name(target_user,False)
        if type(target_user) != str:
            if disconnect:
                self.disconnect_from_database()
            return False
        if gurgle.analyse_full_id(gurgle,target_user) == None:
            target_user = gurgle.make_up_full_id(gurgle,target_user,self.global_domain,None)
        if target_user == None:
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
            self.__mysql_fd.execute("SELECT sub_to FROM subscribed_list_%d WHERE id = '%s'"%(host_user,target_user))
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        tmpVar = self.__mysql_fd.fetchone()
        if disconnect:
            self.disconnect_from_database()
        if tmpVar == None:
            return False
        return bool(tmpVar[0])
    def delete_offline_message(self,userid=None,message_id=0,disconnect=True):
        if userid == None:
            return False
        if type(userid) == str:
            tmpVar = gurgle.analyse_full_id(gurgle,userid)
            if tmpVar == None:
                return False
            try:
                userid = self.get_user_order(tmpVar[1],False)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
            if tmpVar[2] != self.global_domain and tmpVar[2] != self.global_ip:
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
                self.__mysql_fd.execute("DELETE FROM offline_message_%d WHERE message_id='%d'"%(userid,message_id))
            else:
                self.__mysql_fd.execute("DELETE FROM offline_message_%d"%(userid))
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True
    def get_offline_message(self,userid = None,message_type='message',message_id=0,delete=True,limit=100,disconnect=True):
        if userid == None:
            return None
        if type(userid) == str:
            tmpVar = gurgle.analyse_full_id(gurgle,userid)
            if tmpVar == None:
                return None
            if tmpVar[2] != self.global_domain and tmpVar[2] != self.global_ip:
                return None
            try:
                userid = self.get_user_order(tmpVar[1],False)
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
                if message_type != None:
                    self.__mysql_fd.execute("SELECT message_id,message FROM offline_message_list_%d WHERE type='%s' LIMIT %d"
                        %(userid,message_type,limit))
                else:
                    self.__mysql_fd.execute("SELECT message_id,message FROM offline_message_list_%d LIMIT %d"
                        %(userid,limit))
            else:
                if message_type != None:
                    self.__mysql_fd.execute("SELECT message_id,message FROM offline_message_list_%d WHERE type='%s' and message_id='%d' LIMIT %d"
                        %(userid,message_type,message_id,limit))
                else:
                    self.__mysql_fd.execute("SELECT message_id,message FROM offline_message_list_%d WHERE message_id='%d' LIMIT %d"
                        %(userid,message_id,limit))
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        ret_data = []
        id_list = []
        while True:
            data = self.__mysql_fd.fetchone()
            if data == None:
                break
            (msg_id,msg) = data
            ret_data.append(msg)
            id_list.append(msg_id)
        if delete:
            for i in id_list:
                self.__mysql_fd.execute("DELETE FROM offline_message_list_%d WHERE message_id='%d'"%(userid,i))
            try:
                self.__mysql_conn.commit()
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return (id_list,ret_data)
    def subscribe(self,host_user,target_user,disconnect=True):  # from[who send request] to[who will accept request]
        if host_user == None or target_user == None:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(host_user) == str:
            host_user = self.get_user_order(host_user,False)
        if type(host_user) != int:
            if disconnect:
                self.disconnect_from_database()
            return False
        if host_user == 0:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(target_user) == int:
            target_user = self.get_user_name(target_user,False)
        if type(target_user) != str:
            if disconnect:
                self.disconnect_from_database()
            return False
        if gurgle.analyse_full_id(gurgle,target_user) == None:
            target_user = gurgle.make_up_full_id(gurgle,target_user,self.global_domain,None)
        if target_user == None:
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
            self.__mysql_fd.execute("SELECT sub_to FROM subscribed_list_%d WHERE id = '%s'"%(host_user,target_user))
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        tmpVar = self.__mysql_fd.fetchone()
        if tmpVar == None:
            try:
                self.__mysql_fd.execute("INSERT INTO subscribed_list_%d (id,sub_to,sub_from) VALUE ('%s',true,false)"%(host_user,target_user))
                self.__mysql_conn.commit()
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        elif bool(tmpVar[0]) == False:
            try:
                self.__mysql_fd.execute("UPDATE subscribed_list_%d SET sub_to =true WHERE id = '%s'"%(host_user,target_user))
                self.__mysql_conn.commit()
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True
    def unsubscribe(self,host_user,target_user,disconnect=False):
        if host_user == None or target_user == None:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(host_user) == str:
            host_user = self.get_user_order(host_user,False)
        if type(host_user) != int:
            if disconnect:
                self.disconnect_from_database()
            return False
        if host_user == 0:
            if disconnect:
                self.disconnect_from_database()
            return False
        if type(target_user) == int:
            target_user = self.get_user_name(target_user,False)
        if type(target_user) != str:
            if disconnect:
                self.disconnect_from_database()
            return False
        if gurgle.analyse_full_id(gurgle,target_user) == None:
            target_user = gurgle.make_up_full_id(gurgle,target_user,self.global_domain,None)
        if target_user == None:
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
            self.__mysql_fd.execute("SELECT sub_from FROM subscribed_list_%d WHERE id = '%s'"%(host_user,target_user))
        except mysql.Error as err:
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        tmpVar = self.__mysql_fd.fetchone()
        if tmpVar == None:
            return True
        if bool(tmpVar) == False:
            try:
                self.__mysql_fd.execute("DELETE FROM subscribed_list_%d WHERE id='%s'"%(host_user,target_user))
                self.__mysql_conn.commit()
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        else:
            try:
                self.__mysql_fd.execute("UPDATE subscribed_list_%d SET sub_to = false WHERE id='%s'"%(host_user,target_user))
                self.__mysql_conn.commit()
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        if disconnect:
            self.disconnect_from_database()
        return True
if __name__ == '__main__':
    try:
        s = grgl_mysql_controllor()
    except grgl_mysql_controllor_error:
        sys.exit(0)
    while True:
        username = input("Username: ")
        password = input("Password: ")
        s.add_user(username,password)
        if input("Continue?(y/n)").lower() != 'y':
           break
