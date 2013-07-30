#include "../include/linkc_db.h"
#include "../include/server_data_type.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define USER_DB_PATH		"./user.db"
#define FRIEND_DB_PATH		"./friend.db"
#define MAXBUF			1024
#define db_column		2
#define user_c			12

#define DEBUG			1


sqlite3* user_db =   NULL;
sqlite3* friend_db = NULL;

int init_sqlite3_db (void)
{
	extern sqlite3* user_db;
	extern sqlite3* friend_db;
	int result;
	result = sqlite3_open (USER_DB_PATH,&user_db);	//打开数据库
#if DEBUG
        if (result != SQLITE_OK)				//如果失败
        {
                printf ("Open user.db error!\n");
                return -1;
        }
#endif
	result = sqlite3_open (FRIEND_DB_PATH,&friend_db);	//打开数据库
#if DEBUG
        if (result != SQLITE_OK)				//如果失败
        {
                printf ("Open friend.db error!\n");
                return -1;
        }
#endif
	return 1;

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
					return (result);
				}
			}
		}
		errmsg = NULL;
		sqlite3_free_table (dbResult);
		return (0);
	}
	else
	{
		printf ("DB error!\nErrmsg =\t%s\n",errmsg);
		errmsg = NULL;
		sqlite3_free_table (dbResult);
		return -1;
	}
}

friend_data* get_friend_data (int UID, int DestUID, int *STATE)
{
	char exec[MAXBUF];
	char * errmsg = NULL;
	char **dbResult;
	int nRow, nColumn;
	int row,column;
	int result;
	int i;
	sprintf (exec,"SELECT * FROM id%d where id=%d",UID,DestUID);
	result = sqlite3_get_table( friend_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
	if( result == SQLITE_OK )
	{
		if (nRow == 0)
		{
			STATE=0;
			return NULL;
		}
		friend_data *_friend = (friend_data *) malloc (sizeof (friend_data));
		sscanf (dbResult[db_column],"%d",&_friend[i].UID);
		printf ("_friend -> ID = %d\n",_friend->UID);
		strcpy (_friend->nickname,dbResult[db_column * (i+1) + 1]);

		result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
		strcpy (_friend->name,dbResult[user_c + 4 -1]);
		strcpy (_friend->telephone,dbResult[user_c + 5 -1]);
		strcpy (_friend->company,dbResult[user_c + 6 -1]);
		strcpy (_friend->address,dbResult[user_c + 7 -1]);
		return _friend;
	}
}
int get_friends_data (int UID,struct friend_data ** ffb)
{
	char exec[MAXBUF];
	char * errmsg = NULL;
	char **dbResult;
	int nRow, nColumn;
	int row,column;
	int result;
	int i;

	sprintf (exec,"SELECT * FROM id%d",UID);
	result = sqlite3_get_table( friend_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
	if( result == SQLITE_OK )
	{
		struct friend_data* _friend;
		_friend = (struct friend_data *) malloc ((sizeof (struct friend_data))*nRow);
		for (i=0;i<nRow;i++)		// 取得信息
		{
			sscanf (dbResult[db_column * i + db_column],"%d",&_friend[i].UID);
			printf ("_friend[i] -> ID = %d\n",_friend[i].UID);
			strcpy (_friend[i].nickname,dbResult[db_column * (i+1) + 1]);
		}
		printf ("nRow = %d\n",nRow);
		row = nRow;
		for (i=0;i<row;i++)
		{
			sprintf (exec,"SELECT * FROM user WHERE id='%d'",_friend[i].UID);
			printf ("exec = %s\n",exec);
			result = sqlite3_get_table( user_db, exec, &dbResult, &nRow, &nColumn, &errmsg );
			strcpy (_friend[i].name,dbResult[user_c + 4 -1]);
			strcpy (_friend[i].telephone,dbResult[user_c + 5 -1]);
			strcpy (_friend[i].company,dbResult[user_c + 6 -1]);
			strcpy (_friend[i].address,dbResult[user_c + 7 -1]);
#if DEBUG
printf ("The Data Get\t= [%s]\n\t---> %d|%s|%s|%s|%s|%s\n",exec,_friend[i].UID,_friend[i].name,_friend[i].nickname,_friend[i].telephone,_friend[i].company,_friend[i].address);
#endif
		}

		sqlite3_free_table (dbResult);
		errmsg = NULL;
		*ffb = _friend;
		return row;
	}
	sqlite3_free_table (dbResult);
	errmsg = NULL;
	return (-1); 
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

	if (_Flag == 1)	sprintf (exec,"SELECT state FROM user WHERE id='%d'",UID);
	else if (_Flag == 2)	sprintf (exec,"SELECT sockfd FROM user WHERE id='%d'",UID);
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
	return -1;
}

int state_set (int UID,int _sockfd,int _Flag)
{
	
	char exec[MAXBUF];
	char * errmsg = NULL;
	int result;

	if (_Flag > 0)	// 分别为上线，隐身
	{
		sprintf (exec,"UPDATE user SET state='%d' where id='%d'",_Flag,UID);
		result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );
		sprintf (exec,"UPDATE user SET sockfd='%d' where id='%d'",_sockfd,UID);
		result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );
		if( result == SQLITE_OK )	return STATE_OK;
		printf ("Exec Error\t[%s]\n",errmsg);
		return STATE_ERROR;
	}
	if (_Flag == 0)
	{
		sprintf (exec,"UPDATE user SET state='0' where id='%d'",UID);
		result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );
		sprintf (exec,"UPDATE user SET sockfd='0' where id='%d'",UID);
		result = sqlite3_exec( user_db, exec, NULL, NULL,  &errmsg );
		if( result == SQLITE_OK )	return STATE_OK;
		printf ("Exec Error\t[%s]\n",errmsg);
		return STATE_ERROR;
	}
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
			if (result ==  SQLITE_OK)	return 0;
			printf ("Error!\t[%s]\n",errmsg);	
			return -1;
		}
		case LINKC_FRIEND_DELETE:
		{
			sprintf (exec,"DELETE FROM id%d WHERE id='%d'",local_UID,target_ID);
			result = sqlite3_exec( friend_db, exec, NULL, NULL,  &errmsg );
			if (result ==  SQLITE_OK)	return 0;
			printf ("Error!\t[%s]\n",errmsg);	
			return -1;
		}
	}
}
