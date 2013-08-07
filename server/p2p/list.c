#include "def.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

errorcode list_init(list_t *list)
{
	if (list == NULL)
		return ERROR_NULL_ARG;
	
	list->head = NULL;
	list->size = 0;

	return SUCCESS;
}

errorcode list_destroy(list_t *list)
{
	if (list == NULL)
		return ERROR_NULL_ARG;
	
	list_node *node;

	while (list->head != NULL)
	{
		node = list->head;
		list->head = node->next;
		free(node);
	}
	return SUCCESS;
}

errorcode list_find(list_t *list, ip_t SrcIp, void **found,flag_t FLAG)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	list_node *node;
	node = list->head;

	while(1)
	{
		if(node->info->SrcIP==SrcIp)
		{
			if (FLAG==FLAG_ITEM)
				*found = node->info;
			else if(FLAG==FLAG_NODE)
				*found = node;
			return SUCCESS;
		}
		node=node->next;
		if (node==NULL)	break;
	}
	return ERROR_NOT_FOUND;
}

errorcode list_item_add(list_t *list,conn_info item)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);

	list_node *node = (list_node *)malloc(sizeof(list_node));
	conn_info *info = (conn_info *)malloc(sizeof(conn_info));

	*info = item;

	node->info=info;
	node->next=list->head;
	list->head=node;
	list->size++;
	return SUCCESS;
}

errorcode list_node_remove(list_t *list,list_node *node)
{
	CHECK_NOT_NULL(list,ERROR_NULL_ARG);
	CHECK_NOT_NULL(node,ERROR_NULL_ARG);

	list_node *tmp;
	tmp = list->head;

	while(1)
	{
		if(tmp == NULL)	break;
		else if(tmp->next == node)
		{
			tmp->next = node->next;
			free(node->info);
			free(node);
			return SUCCESS;
		}
		tmp = tmp->next;
	}
	return ERROR_NOT_FOUND;
}
