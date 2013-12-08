/* ###########################################################################
 * 登录模块		发送 [1] 表示登录成功，[0] 表示登录失败，[-1] 表示登录出现问题
 * ########################################################################### */

#include "linkc_types.h"
#include "linkc_db.h"
#include "linkc_user.h"
#include "def.h"

int user_login(struct user_data* user)
{
	user->UID = check_pass(user->login);
	if(user->UID == LINKC_FAILURE)
		return LINKC_FAILURE;
	status_set(*user,STATE_ONLINE);
	return LINKC_SUCCESS;
}
