#!/usr/bin/python3
#!encoding='utf-8'
import mysql.connector as mysql
import sys

class database_controllor:
    DATABASE_HOST       = 'localhost'
    DATABASE_USER       = 'root'
    DATABASE_PASS       = '123321123'
    DATABASE_PORT       = 3306
    DATABASE_SUCCESS    = True
    DATABASE_FAILED     = False
    def __init__(self):
        self.__is_connected     = False
        self.__mysql_fd         = None
        self.__mysql_conn       = None
    def is_connected(self):
        return self.__is_connected
    def connect_to_database(self):
        try:
            self.__mysql_conn   = mysql.connect(host = self.DATABASE_HOST,user = self.DATABASE_USER,
                    passwd = self.DATABASE_PASS, port = self.DATABASE_PORT)
        except mysql.Error as e:
            sys.stderr.write('Error connecting to SQL server:%s\n'%e)
            return database_controllor.DATABASE_FAILED
        self.__musql_fd         = self.__mysql_conn.cursor()
        self.__is_connected     = True
        return database_controllor.DATABASE_SUCCESS
    def disconnect_from_database(self):
        if self.is_connected():
            if self.__mysql_fd:
                self.__mysql_fd.close()
            if self.__musql_conn:
                self.__mysql_conn.close()
            self.__is_connected = False
        return database_controllor.DATABASE_SUCCESS
    def auth_password(self,username,password):
        if not self.is_connected():
            if not self.connect_to_database():
                return database_controllor.DATABASE_FAILED
        pass
        self.disconnect_from_database()
        return database_controllor.DATABASE_SUCCESS

