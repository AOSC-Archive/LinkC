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
    AUTH_FAILED         = 10
    AUTH_SUCCESS        = 11
    def __init__(self):
        self.__is_connected     = False
        self.__mysql_fd         = None
        self.__mysql_conn       = None
    def is_connected(self):
        return self.__is_connected
    def connect_to_database(self,db):
        try:
            self.__mysql_conn   = mysql.connect(host = self.DATABASE_HOST,
                    user = self.DATABASE_USER,
                    passwd = self.DATABASE_PASS,
                    port = self.DATABASE_PORT,
                    database = db)
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
        if not self.is_connected():
            if not self.connect_to_database('linkc_users'):
                return grgl_mysql_controllor.DATABASE_FAILED
        self.__mysql_fd.execute("select password from user_info where username = '%s'"%username)
        data = self.__mysql_fd.fetchone()
        self.disconnect_from_database()
        if str(data)[2:-3] == None:
            return grgl_mysql_controllor.AUTH_FAILED
        if str(data)[2:-3] != password:
            return grgl_mysql_controllor.AUTH_FAILED
        return grgl_mysql_controllor.AUTH_SUCCESS

