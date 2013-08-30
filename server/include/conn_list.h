#ifndef _CONN_LIST_H_
#define _CONN_LIST_H_

#include "list.h"
#include "def.h"
#include <pthread.h>

struct conn_list_item_t
{
	conn_info info;	
};
struct conn_list_t
{
	list_t list;
	pthread_mutex_t mutex;
};
typedef struct conn_list_t conn_list;
typedef struct conn_list_item_t conn_list_item;

errorcode conn_list_init(conn_list *list);
errorcode conn_list_destroy(conn_list *list);
errorcode conn_list_add(conn_list *list,conn_list_item item);
errorcode conn_list_find(conn_list *list,conn_info info,port_t *DestPort);
errorcode conn_list_remove(conn_list *list,conn_info info);

#endif
