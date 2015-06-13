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
    def plain_password_authenticate(self,username,password):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            if not self.connect_to_database(
                        grgl_mysql_controllor.DATABASE_NAME):
                return grgl_mysql_controllor.DATABASE_FAILED
        self.__mysql_fd.execute(    \
                "SELECT disabled FROM %s WHERE username = '%s'"
                %(self.USER_INFO_TABLE_NAME,username))
        data = self.__mysql_fd.fetchone()
        if str(data)[1:-2] == None:
            return grgl_mysql_controllor.AUTH_INCORRECT
        if str(data)[1:-2] == '1':
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
        return grgl_mysql_controllor.AUTH_SUCCESS
    def get_user_presence(self,username = None,userid=0,disconnect=True):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            try:
                self.connect_to_database(self.DATABASE_NAME)
            except grgl_mysql_controllor_error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error(err)
        try:
            if username != None:
                self.__mysql_fd.execute(    \
                    "SELECT last_name,first_name,status,mood FROM %s WHERE username = '%s'"
                    %(self.USER_INFO_TABLE_NAME,username))
            elif userid != 0:
                self.__mysql_fd.execute(    \
                    "SELECT last_name,first_name,status,mood FROM %s WHERE id = %d"
                    %(self.USER_INFO_TABLE_NAME,userid))
            else:
                return None
        except mysql.connector.Error as err:
            gurgle.write_log(gurgle,"Database Error : %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            self.disconnect_from_database()
            raise grgl_mysql_controllor_error(err)
        data = self.__mysql_fd.fetchone()
        if disconnect == True:
            self.disconnect_from_database()
        if data is None:
            return None
        (last_name,first_name_name,status,mood) = data
        return (last_name,first_name,status,mood)
    def get_roster(self,username = None,limit=-1):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            if not self.connect_to_database(self.DATABASE_NAME):
                return grgl_mysql_controllor.DATABASE_FAILED
        try:
            self.__mysql_fd.execute("SELECT id FROM %s WHERE username = '%s'"%(self.USER_INFO_TABLE_NAME,username))
        except mysql.Error as err:
            raise grgl_mysql_controllor_error(err)
        data = self.__mysql_fd.fetchone()
        if data == None:
            return None
        user_id = int(data[0])
        try:
            if limit >= 1:
                self.__mysql_fd.execute("SELECT * FROM subscribed_list_%d LIMIT %d"%(user_id,limit))
            else:
                self.__mysql_fd.execute("SELECT * FROM subscribe_list_%d"%user_id)
        except mysql.Error as err:
            raise grgl_mysql_controllor_error(err)
        count = 0
        t_dict = {}
        r_list = []
        k = 0
        while True:
            tmpVar = self.__mysql_fd.fetchone()
            if tmpVar == None:
                break
            count += 1
            t_dict[tmpVar[0]] = tmpVar[1]
        for i in t_dict.keys():
            try:
                tmpVar = self.get_user_presence(None,i,False)
            except mysql.Error as err:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error("Cannot fetch user(id=%d)'s presence"%i)
            if tmpVar == None:
                self.disconnect_from_database()
                raise grgl_mysql_controllor_error("Cannot fetch user(id=%d)'s presence"%i)
            r_list[k] = list(t_dict[i]) + list(tmpVar)
            k+=1
        self.disconnect_from_database()
        return r_list
    def add_user(self,username = None, password = None):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        data = self.get_user_presence(username)
        if data != None:
            gurgle.write_log(gurgle,"Sign up Error [User already exists]",
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(
                    'Failed to add user [User already exists]'
                )
        if password is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            if not self.connect_to_database(self.DATABASE_NAME):
                return grgl_mysql_controllor.DATABASE_FAILED
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
                    "CREATE TABLE subscribe_list_%d (friend_id INT, nickname CHAR(32))"
                    %data[0])
            self.__mysql_conn.commit();
        except mysql.Error as err:
            gurgle.write_log(gurgle,"Mysql Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
        try:
            self.__mysql_fd.execute(    \
                    "CREATE TABLE offline_message_list_%d (type char(16),time DATETIME, message TEXT)"
                    %data[0])
            self.__mysql_conn.commit();
        except mysql.Error as err:
            gurgle.write_log(gurgle,"Mysql Error %s"%err,
                    gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
            raise grgl_mysql_controllor_error(err)
    def update_user_presence(self,username,update_dict):
        if not self.is_connected():
            if not self.connect_to_database(self.DATABASE_NAME):
                return grgl_mysql_controllor.DATABASE_FAILED
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
    def insert_offline_message(self,data,type='message',username = None,userid = 0):
        if data == None:
            return False
        if username == None and userid == 0:
            return False
        if not self.is_connected():
            if not self.connect_to_database(self.DATABASE_NAME):
                return grgl_mysql_controllor.DATABASE_FAILED
        if username != None:
            try:
                self.__mysql_fd.execute(    \
                        "SELECT id FROM %s WHERE username = '%s'"
                        %(self.USER_INFO_TABLE_NAME,username))
            except mysql.Error as err:
                gurgle.write_log(gurgle,"Mysql Error %s"%err,
                        gurgle.GURGLE_LOG_MODE_ERROR,self.__log_level)
                raise grgl_mysql_controllor_error(err)
            userid = self.__mysql_fd.fetchone()
            if userid == None:
                return False
        try:
            self.__mysql_fd.execute(    \
                "INSERT INTO offline_message_list_%d (type,time,message) "
                "VALUES('%s','%s')"
                %(int(userid[0]),type,time.strftime('%Y-%m-%d %X',time.localtime(time.time())),data))
            self.__mysql_conn.commit();
        except mysql.Error as err:
            gurgle.write_log(gurgle,"MySQL Error %s"%err,
                             gurgle.GURGLE_LOG_MODE_ERROR)
            raise grgl_mysql_controllor_error(err)
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
