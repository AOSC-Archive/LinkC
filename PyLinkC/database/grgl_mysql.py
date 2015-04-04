#!/usr/bin/python3
#!encoding='utf-8'
import mysql.connector as mysql
import sys
sys.path.append("..")
from protocol.gurgle import *

class grgl_mysql_controllor:
    DATABASE_HOST       = 'localhost'
    DATABASE_USER       = 'root'
    DATABASE_PASS       = '123321123'
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
    def is_connected(self):
        return self.__is_connected
    def connect_to_database(self,db):
        try:
            self.__mysql_conn   = mysql.connect(    \
                    host = self.DATABASE_HOST,      \
                    user = self.DATABASE_USER,      \
                    passwd = self.DATABASE_PASS,    \
                    port = self.DATABASE_PORT,      \
                    database = db                   \
                )
        except mysql.Error as e:
            gurgle.write_log(e,gurgle.GURGLE_LOG_MODE_ERROR)
            return grgl_mysql_controllor.DATABASE_FAILED
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
    def authenticate(self,username,password):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            if not self.connect_to_database('linkc_users'):
                return grgl_mysql_controllor.DATABASE_FAILED
        self.__mysql_fd.execute(    \
                "select disabled from user_info where username = '%s'"
                %username)
        data = self.__mysql_fd.fetchone()
        if str(data)[1:-2] == None:
            return grgl_mysql_controllor.AUTH_INCORRECT
        if str(data)[1:-2] == '1':
            return grgl_mysql_controllor.AUTH_FAILED
        self.__mysql_fd.execute(    \
                "select password from user_info where username = '%s'"
                %username)
        db_password = self.__mysql_fd.fetchone()
        self.disconnect_from_database()
        if db_password is None:
            return grgl_mysql_controllor.AUTH_INCORRECT
        if db_password[0] != password:
            return grgl_mysql_controllor.AUTH_INCORRECT
        return grgl_mysql_controllor.AUTH_SUCCESS
    def get_user_presence(self,username = None):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            if not self.connect_to_database('linkc_users'):
                return grgl_mysql_controllor.DATABASE_FAILED
        try:
            self.__mysql_fd.execute(    \
                "select status,mood from user_info where username = '%s'"
                %username)
        except mysql.connector.Error as err:
            print ("Database Error : [%s]",err);
            return None
        data = self.__mysql_fd.fetchone()
        if data is None:
            return None
        (status,mood) = data
        if status   == 'None':
            status  = 'null'
        if mood     == 'None':
            mood    = 'null'
        return (status,mood)
    def get_roster(self,username = None):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            if not self.connect_to_database('linkc_users'):
                return grgl_mysql_controllor.DATABASE_FAILED
        self.__mysql_fd.execute(    \
                "select id from user_info where username = '%s'"
                %username)
        data = self.__mysql_fd.fetchone()
        if data is None:
            return None
        print ("id = %s"%data[0])
        self.disconnect_from_database()
    def add_user(self,username = None, password = None):
        if username is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if password is None:
            return grgl_mysql_controllor.ERROR_EMPTY_ARGUMENT
        if not self.is_connected():
            if not self.connect_to_database('linkc_users'):
                return grgl_mysql_controllor.DATABASE_FAILED
        try:
            self.__mysql_fd.execute(    \
                    "insert into user_info (username,password)"
                    "values('%s','%s')"
                    %(username,password)
                    )
            self.__mysql_conn.commit();
        except mysql.Error as err:
            print("Error %s"%err)
        try:
            self.__mysql_fd.execute(    \
                    "select id from user_info where username = '%s'"
                    %username)
        except mysql.Error as err:
            print("Error %s"%err)
        data = self.__mysql_fd.fetchone()
        try:
            self.__mysql_fd.execute(    \
                    "create table id_%d (friend_id int, nickname char(32))"
                    %data[0])
            self.__mysql_conn.commit();
        except mysql.Error as err:
            print("Error %s"%err)

if __name__ == '__main__':
    s = grgl_mysql_controllor()
    s.add_user('test','123321123')
