#include "login.h"

char* login(UserData_t *user)
{
	char *login_data = NULL;
	char url[200] = {0};
	sprintf((char *)url,"https://api.locyanfrp.cn/User/DoLogin?username=%s&password=%s",user->username,user->password);
    login_data = (char *)get_url(url);
    struct json_object *j_login = json_tokener_parse(login_data);
    struct json_object *status = NULL;
    json_object_object_get_ex(j_login, "status", &status);
    if(json_object_get_int(status) >= 0){
        // 保存数据
        handle_data( login_data, user);
        printf("登录成功！\n");
    }
    else{
        printf("账号或密码错误，或者账号不存在\n");
        return "err";
    }
	return login_data;
}
