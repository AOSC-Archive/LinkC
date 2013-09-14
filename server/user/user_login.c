/* ###########################################################################
 * 登录模块		发送 [1] 表示登录成功，[0] 表示登录失败，[-1] 表示登录出现问题
 * ########################################################################### */

#include "linkc_types.h"
#include "linkc_db.h"
#include "linkc_user.h"
#include "def.h"

int user_login(struct user_data* user)
{
	int result;
	user->UID = check_pass(user->login);
	state_set(*user,STATE_ONLINE);
	return 1;
}
