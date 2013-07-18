#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

struct user_data{
	char name [30];		//帐户的名称
	char telephone [20];
    char company [50];
    char address [80];
    char join_time [25];
    char last_login [25];
    char last_ip [10];
};

struct login_data{
    char user_name[12];
    char pass_word[20];
};

typedef struct friend_data
{
        int UID;
        char name [20];
        char nickname [20];             //可以为空，空为“NULL”
        int group;
//-------来自客户端的User_data---------//
        char telephone [20];
        char company [50];
        char address [80];
        char join_time [25];
        char last_login [25];
        char last_ip [10];

}friend_data;



#endif

#ifndef _LINKC_
#define _LINKC_

#define LINKC_OK                "OK"
#define LINKC_FAILURE           "Failure"
#define LINKC_ERROR             "Error"
#define LINKC_FULL              "Full"
#define LINKC_TRY_SO_MANY       "Tried_So_Many"
#define LINKC_TIMED_OUT         "Timed_Out"
#define LINKC_LOGIN             "Action_Login"
#define LINKC_LOGOUT            "Action_Logout"
#define LINKC_GET_FRIEND        "Get_Friend_Data"
#define LINKC_NO_FRIEND         "No_Friend"
#define LINKC_CHAT_WANT         "Chat_Want"
#define LINKC_MESSAGE           "YOUR_MESSAGE"
#define LINKC_QUIT              "QUIT"


#endif
