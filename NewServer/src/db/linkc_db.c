#include "../../include/linkc_def.h"
#include "../../include/linkc_error.h"
#include "../../include/linkc_db.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define db_column           2
#define user_c              12

sqlite3* user_db =   NULL;
sqlite3* friend_db = NULL;

int InitSqliteDb (void){
    extern sqlite3* user_db;
    extern sqlite3* friend_db;
    int result;
    result = sqlite3_open (USER_DB_PATH,&user_db);      //打开数据库
    if (result != SQLITE_OK)
            return LINKC_FAILURE;
    result = sqlite3_open (FRIEND_DB_PATH,&friend_db);  //打开数据库
    if (result != SQLITE_OK)                            //如果失败
            return LINKC_FAILURE;
    return LINKC_SUCCESS;

}

int CheckPassword(LoginData *Data,uint32_t  *UID){
    char exec[STD_BUFFER_SIZE];
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;
    int result;

    sprintf (exec,"SELECT password FROM user WHERE username='%s'",Data->UserName);
    result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
    if( result == SQLITE_OK ){
        if (nRow == 1){
            if ((strncmp (Data->PassWord,dbResult[1],17)) == 0){
                sqlite3_free_table (dbResult);
                sprintf (exec,"SELECT * FROM user WHERE username='%s'",Data->UserName);
                result = sqlite3_get_table (user_db,exec,&dbResult,&nRow,&nColumn,&errmsg); //  获取用户资料
                if (result == SQLITE_OK){
                    sscanf (dbResult[user_c +1 - 1],"%d",&result);  //  录入UID
                    sqlite3_free_table (dbResult);
                    *UID    = result;
                    return result;              //  返回UID
                }
            }
        }
        errmsg = NULL;
        sqlite3_free_table (dbResult);
        return LINKC_FAILURE;
    }else{
        printf ("DB error!\nErrmsg =\t%s\n",errmsg);
        errmsg = NULL;
        sqlite3_free_table (dbResult);
        return LINKC_FAILURE;
    }
}

int GetUserData (int UID, int DestUID ,UserData *User){
    char exec[STD_BUFFER_SIZE];
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;
    int result;
    sprintf (exec,"SELECT * FROM user WHERE id='%d'",DestUID);
    result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
    if( result == SQLITE_OK ){
        if (nRow == 0){
            LinkC_Debug("没有检索到",LINKC_WARNING);
            sqlite3_free_table (dbResult);
            errmsg = NULL;
            return LINKC_FAILURE;
        }
        User->UID = htonl(DestUID);
        strcpy (User->UserName,dbResult[user_c     +   USER_USERNAME]);    // 获取用户名
        strcpy (User->Telephone,dbResult[user_c    +   USER_TEL])    ;     // 获得电话
        strcpy (User->Company,dbResult[user_c      +   USER_COMPANY]);     // 获得公司
        strcpy (User->Address,dbResult[user_c      +   USER_ADDRESS]);     // 获得地址
        User->Status   = atoi(dbResult[user_c      +   USER_STATUS]) ;     // 获得状态
        User->IP       = inet_addr(dbResult[user_c +   USER_LAST_IP]);     // 获得IP地址
        strcpy (User->NickName,dbResult[user_c     +   USER_NAME])   ;     // 获得昵称
        sprintf (exec,"SELECT * FROM id%d WHERE id='%d'",UID,DestUID);              // 检索好友数据库
        result = sqlite3_get_table( friend_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
        if (nRow == 0){                        // 如果不是好友
            sqlite3_free_table (dbResult);
            errmsg = NULL;
            return LINKC_SUCCESS;
        }
        strcpy (User->PrivateNickName,dbResult[db_column + FRIEND_NICKNAME]);// 如果是好友，得到nickname
        sqlite3_free_table (dbResult);
        errmsg = NULL;
        return LINKC_SUCCESS;
    }
    LinkC_Debug("数据库操作错误",LINKC_WARNING);
    return LINKC_FAILURE;        // 返回错误
}

int GetFriendsData (int UID, UserData ** ffb){
    char exec[STD_BUFFER_SIZE];
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;
    int row;
    int result;
    int i;

    sprintf (exec,"SELECT * FROM id%d",UID);
    result = sqlite3_get_table( friend_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
    if( result == SQLITE_OK ){
        UserData* _friend;
        _friend = (UserData*) malloc ((sizeof (UserData))*nRow);
        for (i=0;i<nRow;i++){
            sscanf (dbResult[db_column * i + db_column + FRIEND_UID],"%d",&_friend[i].UID);     // 获得好友UID
            strcpy (_friend[i].PrivateNickName,dbResult[db_column * (i+1) + FRIEND_NICKNAME]);  // 获得好友自定义昵称
        }
        row = nRow;
        for (i=0;i<row;i++){
            sprintf (exec,"SELECT * FROM user WHERE id='%d'",_friend[i].UID);
            _friend[i].UID = htonl(_friend[i].UID);                                             //  转化为网络字节序
            result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
            strcpy (_friend[i].UserName,dbResult[user_c +   USER_USERNAME]);
            strcpy (_friend[i].Telephone,dbResult[user_c+   USER_TEL]);
            strcpy (_friend[i].Company,dbResult[user_c  +   USER_COMPANY]);
            strcpy (_friend[i].Address,dbResult[user_c  +   USER_ADDRESS]);
            _friend[i].IP   = inet_addr(dbResult[user_c +   USER_LAST_IP]);     // 获得IP地址
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

int SetStatus (UserData *user,struct sockaddr_in Addr,int _Flag){
    char exec[STD_BUFFER_SIZE];
    char * errmsg = NULL;
    int result;

    if (_Flag > 0){
        sprintf (exec,"UPDATE user SET state='%d' where id='%d'",_Flag,user->UID);
        result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );        // 更新状态
/*        sprintf (exec,"UPDATE user SET sockfd='%d' where id='%d'",SOCKFD,user->UID);
        result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );        // 更新Sockfd*/
        sprintf (exec,"UPDATE user SET last_ip='%s' where id='%d'",inet_ntoa(Addr.sin_addr),user->UID);
        result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );        // 更新last ip
        if( result == SQLITE_OK )    return LINKC_SUCCESS;
        printf ("Exec Error\t[%s]\n",errmsg);
        return LINKC_FAILURE;
    }else if (_Flag == 0){
        sprintf (exec,"UPDATE user SET state='0' where id='%d'",user->UID);
        result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );        // 更新状态为[离线]
/*        sprintf (exec,"UPDATE user SET sockfd='0' where id='%d'",user->UID);
        result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );        // 更新sockfd为[0]*/
        if( result == SQLITE_OK )    return LINKC_SUCCESS;
        printf ("Exec Error\t[%s]\n",errmsg);
        return LINKC_FAILURE;
    }
    return LINKC_SUCCESS;
}

/*int get_info(int UID,int _Flag){
    char exec[STD_BUFFER_SIZE];
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
    printf ("Error Message is [%s]\n",errmsg);
    errmsg = NULL;
    return LINKC_FAILURE;
}*/

/*int friend_ctl(int local_UID,int target_ID,int _Flag)
{
    char exec[STD_BUFFER_SIZE];
    char * errmsg = NULL;
    int result;
    switch(_Flag){
        case LINKC_FRIEND_ADD:{
            sprintf (exec,"INSERT INTO id%d (id) VALUES ('%d')",local_UID,target_ID);
            result = sqlite3_exec( friend_db, exec, NULL, NULL,  &errmsg );
            if (result ==  SQLITE_OK)    return LINKC_SUCCESS;
            printf ("Error!\t[%s]\n",errmsg);
            return LINKC_FAILURE;
        }case LINKC_FRIEND_DELETE:{
            sprintf (exec,"DELETE FROM id%d WHERE id='%d'",local_UID,target_ID);
            result = sqlite3_exec( friend_db, exec, NULL, NULL,  &errmsg );
            if (result ==  SQLITE_OK)    return LINKC_SUCCESS;
            printf ("Error!\t[%s]\n",errmsg);
            return LINKC_FAILURE;
        }
    }
    return LINKC_SUCCESS;
}

int get_user_info(int UID, UserData **info){
    char exec[STD_BUFFER_SIZE];
    char * errmsg = NULL;
    char **dbResult;
    int nRow, nColumn;
    int result;
    sprintf (exec,"SELECT * FROM user WHERE id=%d",UID);
    result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
    printf("Exec [%s]\n",exec);
    if(result == SQLITE_OK ){
        if (nRow == 0){
            sqlite3_free_table (dbResult);
            errmsg = NULL;
            return LINKC_FAILURE;
        }
        UserData *t = (UserData*)malloc(sizeof(UserData));
        strcpy (t->UserName,dbResult[user_c  +    USER_USERNAME]);
        strcpy (t->Telephone,dbResult[user_c +    USER_TEL]);
        strcpy (t->Company,dbResult[user_c   +    USER_COMPANY]);
        strcpy (t->Address,dbResult[user_c   +    USER_ADDRESS]);

        *info = t;
        sqlite3_free_table (dbResult);
        return LINKC_SUCCESS;
    }
    printf("Error!\n");
    sqlite3_free_table (dbResult);
    return LINKC_FAILURE;
}*/
