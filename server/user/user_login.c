/* ###########################################################################
 * 登录模块		发送 [1] 表示登录成功，[0] 表示登录失败，[-1] 表示登录出现问题
 * ########################################################################### */

#include "../include/server_data_type.h"
#include "../include/linkc_db.h"
#include "../include/linkc_user.h"

int user_login(struct user_data* user)
{
	printf ("Login mode\n");
	int result;

	printf ("Name=%s\nPass=%s\n",user -> login.user_name,user -> login.pass_word);

	user -> UID = check_pass (user -> login);
	if (user -> UID > 0)
	{
		if ((state_set(*user,STATE_ONLINE)) == STATE_ERROR)	return (-1);
		return (1);
	}
	else if(user -> UID == 0)
		return 0;
	else
		return -1;
}
