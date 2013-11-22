/* ###################################################
 * # 注销一个用户	参数1 [用户UID]	参数2 [用户连接的Sockfd]
 * ###################################################*/


#include "linkc_types.h"
#include "linkc_db.h"
#include "linkc_user.h"
#include "def.h"

int user_logout(struct user_data user)
{
	CHECK_FAILED(status_set (user,STATE_OFFLINE),ERROR_SET_STATE);	// 设置下线
	return SUCCESS;
}
