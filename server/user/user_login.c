/* ###########################################################################
 * 登录模块		发送 [1] 表示登录成功，[0] 表示登录失败，[-1] 表示登录出现问题
 * ########################################################################### */

#include "server_data_type.h"
#include "linkc_db.h"
#include "linkc_user.h"
#include "def.h"

int user_login(struct user_data* user)
{
	int result;
	CHECK_FAILED(user->UID = check_pass(user->login),ERROR_LOGIN);
	CHECK_FAILED(state_set(*user,STATE_ONLINE),ERROR_SET_STATE);
	return (SUCCESS);
}
