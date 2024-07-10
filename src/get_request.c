#include "get_request.h"

// 回调函数，用于处理从服务器获取的数据
size_t get_data(char *buffer, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    char **data = (char **)userp; // 将 userp 强制转换为 char* 类型的指针

    if (*data == NULL) {
        // 初次分配内存
        *data = (char *)malloc(total_size + 1);
        if (*data == NULL) {
            fprintf(stderr, "Not enough memory (malloc returned NULL)\n");
            return 0;
        }
        memcpy(*data, buffer, total_size);
        (*data)[total_size] = '\0';
    } else {
        // 重新为 data 分配空间
        char *new_data = realloc(*data, strlen(*data) + total_size + 1);
        if (new_data == NULL) {
            fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
            free(*data);
            *data = NULL;
            return 0;
        }
        strcat(new_data, buffer);
        *data = new_data;
    }

    return total_size;
}

// 获取 URL 内容
const char *get_url(const char *url) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init failed\n");
        return NULL;
    }

    char *data = NULL;
    curl_easy_setopt(curl, CURLOPT_URL, url); // 设置链接
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data); // 设置回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data); // 使用 data 指针

    CURLcode ret = curl_easy_perform(curl);
    if (ret != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform failed: %s\n", curl_easy_strerror(ret));
        free(data); // 确保释放之前分配的内存
        curl_easy_cleanup(curl);
        return "err";
    }

    curl_easy_cleanup(curl);
    return data;
}
