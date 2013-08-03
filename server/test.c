#include <stdio.h>
#include "include/linkc_db.h"
#include "include/server_data_type.h"

int main(void)
{
	int i;
	init_sqlite3_db();	
	struct friend_data *s= get_friend_data(1,4,&i);
	printf("IP = %s\n",s->ip);
	return 0;
}
