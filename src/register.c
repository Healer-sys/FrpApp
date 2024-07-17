#include "register.h"

// 初始化注册结构体
Register_t* InitRegister() {
    Register_t* register_t = (Register_t*)malloc(sizeof(Register_t));
    register_t->username = NULL;
    register_t->password = NULL;
    register_t->confirmPassword = NULL;
    register_t->email = NULL;
    register_t->verify = NULL;
    register_t->qq = NULL;
    return register_t;
}

// 注册用户
void register_user(void) {
    Register_t* newuser = InitRegister();
    if (newuser->username == NULL) {
        printf("请输入用户名(ESC退出)：");
        scanf("%s", newuser->username);
    }

    if (newuser->password == NULL) {
        printf("请输入密码：");
        scanf("%s", newuser->password);
    }
    while (strlen(newuser->password) < 6) {
        printf("密码长度不能小于6位，请重新输入\n");
        scanf("%s", newuser->password);
    }
    if (newuser->confirmPassword == NULL) {
        printf("请再次输入密码：");
        scanf("%s", newuser->confirmPassword);
    }
    printf("请再次输入密码：");
    scanf("%s", newuser->confirmPassword);
    while (strcmp(newuser->password, newuser->confirmPassword) != 0) {
        printf("两次密码输入不一致，请重新输入\n");
        scanf("%s", newuser->confirmPassword);
    }
    if (newuser->email == NULL) {
        printf("请输入邮箱：");
        scanf("%s", newuser->email);
    }
    if (newuser->qq == NULL) {
        printf("请输入QQ：");
        scanf("%s", newuser->qq);
    }
    if (newuser->verify == NULL) {
        requestCode(newuser->email);
        printf("请输入验证码：");
        scanf("%s", newuser->verify);
    }
    requestRegister(newuser->username, newuser->password, newuser->confirmPassword, newuser->email, newuser->verify, newuser->qq);
}


/*
curl -X POST \
     -F "username=testuser" \
     -F "password=mypassword" \
     -F "confirm_password=myconfirmpassword" \
     -F "email=test@example.com" \
     -F "verify=myverificationcode" \
     -F "qq=123456789" \
    "https://api-v2.locyanfrp.cn/api/v2"
*/

// 提交注册
int requestRegister(const char *username, const char *password, const char *confirmPassword, const char *email, const char *verify, const char *qq) {
    CURL *curl;
    CURLcode res;
    char *readBuffer = NULL;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api-v2.locyanfrp.cn/api/v2/users/register");
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        struct curl_httppost *formpost=NULL;
        struct curl_httppost *lastptr=NULL;
        struct curl_slist *headerlist=NULL;

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "username",
                     CURLFORM_COPYCONTENTS, username,
                     CURLFORM_END);

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "password",
                     CURLFORM_COPYCONTENTS, password,
                     CURLFORM_END);

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "confirm_password",
                     CURLFORM_COPYCONTENTS, confirmPassword,
                     CURLFORM_END);

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "email",
                     CURLFORM_COPYCONTENTS, email,
                     CURLFORM_END);

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "verify",
                     CURLFORM_COPYCONTENTS, verify,
                     CURLFORM_END);

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "qq",
                     CURLFORM_COPYCONTENTS, qq,
                     CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        // 设置回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // 执行请求
        res = curl_easy_perform(curl);

        // 输出错误信息
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
        }

        // 释放curl资源
        curl_easy_cleanup(curl);
        curl_formfree(formpost);
        curl_slist_free_all(headerlist);
    }

    //printf("%s\n", readBuffer);
    if (strstr(readBuffer, "\"status\":200") != NULL) {
        printf("注册成功\n");
    } else {
        printf("注册失败\n");
    }
    curl_global_cleanup();
    free(readBuffer);
    return 0;
}
// 发送验证码
void requestCode(const char* email) {
    char* url = (char*)malloc(sizeof(char) * 100);
    snprintf(url, 100, "https://api-v2.locyanfrp.cn/api/v2/users/send?email=%s", email);
    char* requestcode = (char*)get_url(url);
    free(url);
    free(requestcode);
}

