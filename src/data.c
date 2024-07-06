#include "data.h"
//处理数据

void HandleUserData(char* data, UserData_t *user){
    //const char *file_string;
    //UserData user;
    FILE *fd = NULL;
    fd = fopen("user.json","w+");

    //创建并读string到json
    struct json_object *key = NULL;
    key = json_tokener_parse( data );
    
    //创建userdata的json
    struct json_object *j_userdata = NULL;

    //重置key
    struct json_object *val = NULL;
    struct json_object *j_userdata_key = json_object_new_object();

    //取token
    if(json_object_object_get_ex(key, "token", &val)){
        user->token = (char *)json_object_get_string(val);
        json_object_object_add(j_userdata_key, "token", json_object_get(val));        
    }
    //取userdata
    if(json_object_object_get_ex(key,"userdata",&j_userdata)){
        //拿username
        json_object_object_get_ex(j_userdata, "username", &val);
        user->username = (char *)json_object_get_string(val);
        json_object_object_add(j_userdata_key, "username", json_object_get(val));
        //拿email
        json_object_object_get_ex(j_userdata, "email", &val);
        json_object_object_add(j_userdata_key, "email", json_object_get(val));
        //拿frptoken
        json_object_object_get_ex(j_userdata, "frptoken", &val);
        user->frptoken = (char *)json_object_get_string(val);
        json_object_object_add(j_userdata_key, "frptoken", json_object_get(val));        
    }
    //写文件
    
    fwrite(json_object_get_string(j_userdata_key), 1,strlen(json_object_get_string(j_userdata_key)),fd);
    //释放
    json_object_put(j_userdata_key);
    fclose(fd);
    fd = NULL;
}