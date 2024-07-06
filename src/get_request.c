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
	CURL *curl = NULL;
	const char *data = NULL;
	CURLcode ret;
    curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);                       //设置链接
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data);        //设置回调函数
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);               //回调指针
		ret = curl_easy_perform(curl);                                  //请求
	}
	//清除url句柄
	curl_easy_cleanup(curl);
	curl = NULL;
	return data;
}