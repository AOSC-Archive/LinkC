/*
 * Author           ： Junfeng Zhang <564691478@qq.com>
 * Last-Change      ： April 5, 2014
 */

#include "def.h"
#include "linkc_db.h"
#include "linkc_types.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define db_column        2
#define user_c            12

#define DEBUG            1


sqlite3* user_db =   NULL;
sqlite3* friend_db = NULL;

int init_sqlite3_db (void)
{
    extern sqlite3* user_db;
    extern sqlite3* friend_db;
    int result;
    result = sqlite3_open (USER_DB_PATH,&user_db);  //打开数据库
    if (result != SQLITE_OK)                        //如果失败
    {
#if DEBUG
            printf ("Open user.db error!\n");
#endif
            return LINKC_FAILURE;
    }
    result = sqlite3_open (FRIEND_DB_PATH,&friend_db);  //打开数据库
    if (result != SQLITE_OK)                            //如果失败
    {
#if DEBUG
            printf ("Open friend.db error!\n");
#endif
            return LINKC_FAILURE;
    }
    return LINKC_SUCCESS;

}

int check_pass(struct login_data user)
{
    char exec[MAXBUF];
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;
    int result;

    sprintf (exec,"SELECT password FROM user WHERE username='%s'",user.user_name);
#if DEBUG
printf ("The password check\t= %s\n",exec);
#endif
    result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
    if( result == SQLITE_OK )
    {
        if (nRow == 1)
        {
            if ((strncmp (user.pass_word,dbResult[1],20)) == 0)
            {
                sqlite3_free_table (dbResult);
                sprintf (exec,"SELECT * FROM user WHERE username='%s'",user.user_name);
                result = sqlite3_get_table (user_db,exec,&dbResult,&nRow,&nColumn,&errmsg);
                if (result == SQLITE_OK)
                {
                    sscanf (dbResult[user_c +1 - 1],"%d",&result);
                    sqlite3_free_table (dbResult);
                    return result;
                }
            }
        }
        errmsg = NULL;
        sqlite3_free_table (dbResult);
        return LINKC_FAILURE;
    }
    else
    {
        printf ("DB error!\nErrmsg =\t%s\n",errmsg);
        errmsg = NULL;
        sqlite3_free_table (dbResult);
        return LINKC_FAILURE;
    }
}

int get_friend_data (int UID, int DestUID ,struct friend_data **_ffb)
{
    char exec[MAXBUF];
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;
    int result;
    struct friend_data* _friend;
    _friend = (struct friend_data *) malloc (sizeof (struct friend_data));
    sprintf (exec,"SELECT * FROM user WHERE id='%d'",DestUID);
    printf("Friend Data Get %s\n",exec);
    result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
    if( result == SQLITE_OK )
    {
        printf("OK\n");
        if (nRow == 0)
        {
            sqlite3_free_table (dbResult);
            errmsg = NULL;
            return LINKC_FAILURE;
        }
        _friend[0].info.UID = DestUID;
        strcpy (_friend[0].info.username,dbResult[user_c    +   USER_USERNAME]);    // 获取用户名
        strcpy (_friend[0].info.telephone,dbResult[user_c   +   USER_TEL])    ;     // 获得电话
        strcpy (_friend[0].info.company,dbResult[user_c     +   USER_COMPANY]);     // 获得公司
        strcpy (_friend[0].info.address,dbResult[user_c     +   USER_ADDRESS]);     // 获得地址
        _friend[0].info.status    = atoi(dbResult[user_c    +   USER_STATUS]) ;     // 获得状态
        _friend[0].info.ip    = inet_addr(dbResult[user_c   +   USER_LAST_IP]);     // 获得IP地址
        strcpy (_friend[0].nickname,dbResult[user_c         +   USER_NAME])   ;     // 获得昵称
            printf("Ip = %d\n",_friend[0].info.ip);
        sprintf (exec,"SELECT * FROM id%d WHERE id='%d'",UID,DestUID);              // 检索好友数据库
        result = sqlite3_get_table( friend_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
        if (nRow == 0)                        // 如果不是好友
        {

            sqlite3_free_table (dbResult);
            errmsg = NULL;
            return LINKC_FAILURE;
        }
        strcpy (_friend[0].nickname,dbResult[db_column +    FRIEND_NICKNAME]);// 如果是好友，得到nickname
        sqlite3_free_table (dbResult);
        *_ffb = _friend;
        errmsg = NULL;
        return LINKC_SUCCESS;
    }
        printf("Not OK\n");
    return LINKC_FAILURE;        // 返回错误
}
int get_friends_data (int UID,struct friend_data ** ffb)
{
    char exec[MAXBUF];
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;
    int row;
    int result;
    int i;

    sprintf (exec,"SELECT * FROM id%d",UID);
    result = sqlite3_get_table( friend_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
    if( result == SQLITE_OK )
    {
        struct friend_data* _friend;
        _friend = (struct friend_data *) malloc ((sizeof (struct friend_data))*nRow);
        for (i=0;i<nRow;i++)        // 取得信息
        {
            sscanf (dbResult[db_column * i + db_column + FRIEND_UID],"%d",&_friend[i].info.UID);    // 获得好友UID
            printf ("_friend[i].ID = %d\n",_friend[i].info.UID);
            strcpy (_friend[i].nickname,dbResult[db_column * (i+1) + FRIEND_NICKNAME]);        // 获得好友自定义昵称
        }
        printf ("nRow = %d\n",nRow);
        row = nRow;
        for (i=0;i<row;i++)
        {
            sprintf (exec,"SELECT * FROM user WHERE id='%d'",_friend[i].info.UID);
            printf ("exec = %s\n",exec);
            result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
            strcpy (_friend[i].info.username,dbResult[user_c  +    USER_USERNAME]);
            strcpy (_friend[i].info.telephone,dbResult[user_c +    USER_TEL]);
            strcpy (_friend[i].info.company,dbResult[user_c   +    USER_COMPANY]);
            strcpy (_friend[i].info.address,dbResult[user_c   +    USER_ADDRESS]);
            _friend[i].info.ip=0;
#if DEBUG
printf ("The Data Get\t= [%s]\n\t---> %d|%s|%s|%s|%s|%s\n",exec,_friend[i].info.UID,_friend[i].info.username,_friend[i].nickname,_friend[i].info.telephone,_friend[i].info.company,_friend[i].info.address);
#endif
        }

        sqlite3_free_table (dbResult);
        errmsg = NULL;
        *ffb = _friend;
        return row;
    }
    sqlite3_free_table (dbResult);
    errmsg = NULL;
    return LINKC_FAILURE;
}

void linkc_free_data (struct friend_data ** ffb,int _count)
{
    free (*ffb);
}

int get_info(int UID,int _Flag)
{
    char exec[MAXBUF];
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;
    int result;

    if (_Flag == 1)    sprintf (exec,"SELECT state FROM user WHERE id='%d'",UID);
    else if (_Flag == 2)    sprintf (exec,"SELECT sockfd FROM user WHERE id='%d'",UID);
    result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
    if( result == SQLITE_OK )
    {
        sscanf (dbResult[1],"%d",&result);
        sqlite3_free_table (dbResult);
        errmsg = NULL;
        return result;
    }
    sqlite3_free_table (dbResult);
#if DEBUG
    printf ("Error Message is [%s]\n",errmsg);
#endif
    errmsg = NULL;
    return LINKC_FAILURE;
}

int status_set (struct user_data *user,int _Flag)
{

    char exec[MAXBUF];
    char * errmsg = NULL;
    int result;

    if (_Flag > 0)    // 分别为上线，隐身
    {
        sprintf (exec,"UPDATE user SET state='%d' where id='%d'",_Flag,user->UID);
        result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );        // 更新状态
        sprintf (exec,"UPDATE user SET sockfd='%d' where id='%d'",user->sockfd,user->UID);
        result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );        // 更新Sockfd
        sprintf (exec,"UPDATE user SET last_ip='%s' where id='%d'",inet_ntoa(user->addr.sin_addr),user->UID);
        result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );        // 更新last ip
        if( result == SQLITE_OK )    return LINKC_SUCCESS;
        printf ("Exec Error\t[%s]\n",errmsg);
        return LINKC_FAILURE;
    }
    if (_Flag == 0)    // 设置成下线
    {
        sprintf (exec,"UPDATE user SET state='0' where id='%d'",user->UID);
        result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );        // 更新状态为[离线]
        sprintf (exec,"UPDATE user SET sockfd='0' where id='%d'",user->UID);
        result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );        // 更新sockfd为[0]
        if( result == SQLITE_OK )    return LINKC_SUCCESS;
        printf ("Exec Error\t[%s]\n",errmsg);
        return LINKC_FAILURE;
    }
    return LINKC_SUCCESS;
}

int friend_ctl(int local_UID,int target_ID,int _Flag)
{
    char exec[MAXBUF];
    char * errmsg = NULL;
    int result;
    switch(_Flag)
    {
        case LINKC_FRIEND_ADD:
        {
            sprintf (exec,"INSERT INTO id%d (id) VALUES ('%d')",local_UID,target_ID);
            result = sqlite3_exec( friend_db, exec, NULL, NULL,  &errmsg );
            if (result ==  SQLITE_OK)    return LINKC_SUCCESS;
            printf ("Error!\t[%s]\n",errmsg);
            return LINKC_FAILURE;
        }
        case LINKC_FRIEND_DELETE:
        {
            sprintf (exec,"DELETE FROM id%d WHERE id='%d'",local_UID,target_ID);
            result = sqlite3_exec( friend_db, exec, NULL, NULL,  &errmsg );
            if (result ==  SQLITE_OK)    return LINKC_SUCCESS;
            printf ("Error!\t[%s]\n",errmsg);
            return LINKC_FAILURE;
        }
    }
    return LINKC_SUCCESS;
}

int get_user_info(int UID, struct user_info **info)
{
    char exec[MAXBUF];
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;
    int result;
    sprintf (exec,"SELECT * FROM user WHERE id=%d",UID);
    result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
    printf("Exec [%s]\n",exec);
    if(result == SQLITE_OK )
    {
        if (nRow == 0)
        {
            sqlite3_free_table (dbResult);
            errmsg = NULL;
            return LINKC_FAILURE;
        }
        struct user_info *t = (struct user_info*)malloc(sizeof(struct user_info));
        strcpy (t->username,dbResult[user_c  +    USER_USERNAME]);
        strcpy (t->telephone,dbResult[user_c +    USER_TEL]);
        strcpy (t->company,dbResult[user_c   +    USER_COMPANY]);
        strcpy (t->address,dbResult[user_c   +    USER_ADDRESS]);
        
        *info = t;
        sqlite3_free_table (dbResult);
        return LINKC_SUCCESS;
    }
    printf("Error!\n");
    sqlite3_free_table (dbResult);
    return LINKC_FAILURE;
}
