#include "list.h"
#include "conn_list.h"
#include "def.h"
#include <string.h>
#include <stdio.h>

errorcode conn_list_init(conn_list *list)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	CHECK_FAILED(list_init(&list->list),ERROR_LIST_INIT);
	if (pthread_mutex_init(&(list->mutex),NULL)<0)
		return NOT_OK;

	return SUCCESS;
}

errorcode conn_list_destroy(conn_list *list)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	CHECK_FAILED(pthread_mutex_lock(&(list->mutex)),ERROR_MUTEX_LOCK);	//上锁
	CHECK_FAILED(list_destroy(&(list->list)),ERROR_LIST_DESTORY);		//销毁信息库
	CHECK_FAILED(pthread_mutex_unlock(&(list->mutex)),ERROR_MUTEX_UNLOCK);	//解锁
	CHECK_FAILED(pthread_mutex_destroy(&(list->mutex)),ERROR_MUTEX_DESTORY);//销毁互斥锁

	return SUCCESS;
}

errorcode conn_list_add(conn_list *list,conn_list_item item)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	CHECK_FAILED(pthread_mutex_lock(&(list->mutex)),ERROR_MUTEX_LOCK);
	CHECK_FAILED(list_item_add(&(list->list),item.info),ERROR_ITEM_ADD);
	CHECK_FAILED(pthread_mutex_unlock(&(list->mutex)),ERROR_MUTEX_UNLOCK);	//解锁

	return SUCCESS;
}

errorcode conn_list_find(conn_list *list,conn_info info,port_t *DestPort)
{
	port_t k;
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	CHECK_FAILED(pthread_mutex_lock(&(list->mutex)),ERROR_MUTEX_LOCK);
	int i = list_connection_find(&(list->list),info,&k,FLAG_ITEM);
	if (i == -14)		// If Not Found
	{
		CHECK_FAILED(pthread_mutex_unlock(&(list->mutex)),ERROR_MUTEX_UNLOCK);	//解锁
		return -14;	// Return Not Found
	}
	CHECK_FAILED(pthread_mutex_unlock(&(list->mutex)),ERROR_MUTEX_UNLOCK);	//解锁
	*DestPort = k;

	return SUCCESS;
}

errorcode conn_list_remove(conn_list *list, conn_info info)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	CHECK_FAILED(pthread_mutex_lock(&(list->mutex)),ERROR_MUTEX_LOCK);
	int i = list_node_remove(&(list->list),info);
	if (i == -14)
	{
		return NOT_OK;
	}
	CHECK_FAILED(pthread_mutex_unlock(&(list->mutex)),ERROR_MUTEX_UNLOCK);	//解锁
	return SUCCESS;
}
