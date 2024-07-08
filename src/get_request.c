#include "get_request.h"

size_t get_data(char *duffer, size_t size, size_t nmemd, void **userp){
	size_t bytes = size * nmemd;
    // 将 userp 空指针强制转换为 char* 类型的指针
    char *data = (char*)(*userp);
    // 重新为 data 分配空间
    char *new_data = realloc(data, bytes + 1);
    // 判断是否开辟空间成功
    if (new_data == NULL) {
        // 如果不成功
        fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
        return 0;
    }
    // 将获取的内容拷贝到 data 中
    memcpy(new_data, duffer, bytes);
    // 在 data 中的最后位置赋予一个 '\0'
    new_data[bytes] = '\0';
    // 将新分配的指针返回给调用者
    *userp = new_data;
    return bytes;
}

const char *get_url(const char *url){
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init failed\n");
        return NULL;
    }

    char *data = NULL; // 使用可修改的字符数组
    CURLcode ret;
    curl_easy_setopt(curl, CURLOPT_URL, url); // 设置链接
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data); // 设置回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data); // 使用data指针

    ret = curl_easy_perform(curl);
    if (ret != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform failed: %s\n", curl_easy_strerror(ret));
        // 在出错时清理
        free(data); // 确保释放之前分配的内存
        curl_easy_cleanup(curl);
        return "err";
    }

    // 成功后清除url句柄，但不释放data，因为它可能被外部使用
    curl_easy_cleanup(curl);
    curl = NULL;
    return data;
}