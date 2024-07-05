#include "get.h"


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
		curl_easy_setopt(curl, CURLOPT_URL, url);//设置链接
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_data);//设置回调函数
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);//回调指针
		ret = curl_easy_perform(curl);//请求
	}
	//清除url句柄
	curl_easy_cleanup(curl);
	curl = NULL;
	return data;
}

//获取服务器列表并保留自己有的服务器
int GetServerList(const char *username, const char *token, const char *frptoken) {

    FILE *fd = NULL;
    fd = fopen("me.ini","w+");

    char GetProxiesList_Url[200]  = {0};

    //拼接用户节点信息url
    sprintf((char *)GetProxiesList_Url,"https://api.locyanfrp.cn/Proxies/GetProxiesList?username=%s&token=%s",username,token);
    struct json_object *j_GetProxiesList = json_tokener_parse( get_url(GetProxiesList_Url) );
    if (j_GetProxiesList == NULL) {
        fprintf(stderr, "Error parsing JSON j_GetProxiesList\n");
        return 1;
    }

    //服务器列表
    struct json_object *j_GetServerList = json_tokener_parse( get_url("https://api.locyanfrp.cn/Proxies/GetServerList") );
    //printf("%s",json_object_get_string(j_GetServerList));
    if (j_GetServerList == NULL) {
        fprintf(stderr, "Error parsing JSON j_GetServerList\n");
        //释放前面的j_GetProxiesList的json
        json_object_put(j_GetProxiesList);
        return 1;
    }

    if (json_object_object_get_ex(j_GetProxiesList, "proxies", &j_GetProxiesList) == 0) {
        fprintf(stderr, "Error getting proxies array from JSON j_GetProxiesList\n");
        json_object_put(j_GetProxiesList);
        json_object_put(j_GetServerList);
        return 1;
    }
    printf("proxies is %s \n",json_object_get_string(j_GetProxiesList));

    int array_length_server = json_object_array_length(j_GetProxiesList);
    printf("隧道个数 = %d \n",array_length_server);

    int node_value_last = 0;
    //外层循环隧道列表
    for (int i = 0; i < array_length_server; i++) {

        char data[100] = {0};
        json_object *modf_temp =NULL;
        json_object *proxy1 = json_object_array_get_idx(j_GetProxiesList, i);

        // 获取proxy1的node
        json_object *node1 = NULL;
        if (json_object_object_get_ex(proxy1, "node", &node1) == 0) {
            fprintf(stderr, "Error getting id from proxy in JSON 1\n");
            json_object_put(j_GetProxiesList);
            json_object_put(j_GetServerList);
            return 1;
        }

        // 获取id1的整数值
        int node_value_now = json_object_get_int(node1);
        if(node_value_now == node_value_last) {
            fwrite("\n", 1, 2, fd);

            json_object_object_get_ex(proxy1, "proxy_name", &modf_temp);
            sprintf((char *)data,"[%s]\n",json_object_get_string(modf_temp));
            fwrite(data, 1, strlen(data), fd);

            fwrite("privilege_mode = true\n", 1, 23, fd);

            json_object_object_get_ex(proxy1, "proxy_type", &modf_temp);
            sprintf((char *)data,"type = %s\n",json_object_get_string(modf_temp));
            fwrite(data, 1, strlen(data), fd);

            json_object_object_get_ex(proxy1, "local_ip", &modf_temp);
            sprintf((char *)data,"local_ip = %s\n",json_object_get_string(modf_temp));
            fwrite(data, 1, strlen(data), fd);

            json_object_object_get_ex(proxy1, "local_port", &modf_temp);
            sprintf((char *)data,"local_port = %s\n",json_object_get_string(modf_temp));
            fwrite(data, 1, strlen(data), fd);

            json_object_object_get_ex(proxy1, "remote_port", &modf_temp);
            sprintf((char *)data,"remote_port = %s\n",json_object_get_string(modf_temp));
            fwrite(data, 1, strlen(data), fd);
        }
        else {

            // 遍历proxies2数组
            int array_length2 = json_object_array_length(j_GetServerList);
            printf("服务器数量 = %d\n", array_length2);
            for (int j = 0; j < array_length2; j++) {
                json_object *proxy2 = json_object_array_get_idx(j_GetServerList, j);

                // 获取proxy2的id
                json_object *id2 = NULL;
                if (json_object_object_get_ex(proxy2, "id", &id2) == 0) {
                    fprintf(stderr, "Error getting id from proxy in JSON 2\n");
                    json_object_put(j_GetProxiesList);
                    json_object_put(j_GetServerList);
                    return 1;
                }

                // 获取id2的整数值
                int id_value2 = json_object_get_int(id2);

                // 检查id是否匹配
                if (node_value_now == id_value2) {
                    // 处理匹配的数据
                    fwrite("[common]\n", 1, 10, fd);
                    //写server_addr
                    json_object_object_get_ex(proxy2, "hostname", &modf_temp);
                    sprintf((char *)data,"server_addr = %s\n",json_object_get_string(modf_temp));
                    fwrite(data, 1, strlen(data), fd);

                    //写server_port
                    fwrite("server_port = 2333\n", 1, 20, fd);

                    //写tcp_mux
                    fwrite("tcp_mux = true\n", 1, 16, fd);
                    fwrite("protocol = tcp\n", 1, 16, fd);
                    fwrite("tcp_mux = true\n", 1, 16, fd);
                    //写user
                    sprintf((char *)data,"user = %s\n",frptoken);
                    fwrite(data, 1, strlen(data), fd);
                    //写token
                    fwrite("token = LoCyanToken\n", 1, 21, fd);
                    fwrite("dns_server = 223.6.6.6\n", 1, 24, fd);
                    fwrite("tls_enable = false\n", 1, 20, fd);
                    fwrite("\n", 1, 2, fd);

                    json_object_object_get_ex(proxy1, "proxy_name", &modf_temp);
                    sprintf((char *)data,"[%s]\n",json_object_get_string(modf_temp));
                    fwrite(data, 1, strlen(data), fd);
                    fwrite("privilege_mode = true\n", 1, 23, fd);

                    json_object_object_get_ex(proxy1, "proxy_type", &modf_temp);
                    sprintf((char *)data,"type = %s\n",json_object_get_string(modf_temp));
                    fwrite(data, 1, strlen(data), fd);

                    json_object_object_get_ex(proxy1, "local_ip", &modf_temp);
                    sprintf((char *)data,"local_ip = %s\n",json_object_get_string(modf_temp));
                    fwrite(data, 1, strlen(data), fd);

                    json_object_object_get_ex(proxy1, "local_port", &modf_temp);
                    sprintf((char *)data,"local_port = %s\n",json_object_get_string(modf_temp));
                    fwrite(data, 1, strlen(data), fd);

                    json_object_object_get_ex(proxy1, "remote_port", &modf_temp);
                    sprintf((char *)data,"remote_port = %s\n",json_object_get_string(modf_temp));
                    fwrite(data, 1, strlen(data), fd);
                }
            }
        }
        node_value_last = node_value_now;
    }

    json_object_put(j_GetProxiesList);
    json_object_put(j_GetServerList);

    fclose(fd);
    fd = NULL;
    
    return 0;
}

/*
    显示节点信息
*/
void ShowNode(char *username,char *token)
{
    int count;
    char GetProxiesList_Url[200];

    sprintf((char *)GetProxiesList_Url,"https://api.locyanfrp.cn/Proxies/GetProxiesList?username=%s&token=%s",username,token);
    struct json_object *j_GetProxiesList = json_tokener_parse( get_url(GetProxiesList_Url) );
    struct json_object *temp;
    //printf("j_GetProxiesList is %s\n", json_object_get_string(j_GetProxiesList));
    json_object_object_get_ex(j_GetProxiesList,"count",&temp);
    //printf("count is %s\n", json_object_get_string(temp));
    count = json_object_get_int(temp);
    json_object_object_get_ex(j_GetProxiesList, "proxies", &j_GetProxiesList);
    //printf("proxies is %s\n", json_object_get_string(j_GetProxiesList));
    for(int i = 0; i < count; i++)
    {
        json_object *proxy1 = json_object_array_get_idx(j_GetProxiesList, i);
        json_object_object_get_ex(proxy1,"proxy_name",&temp);
        printf("节点%d:\n", i+1);
        printf("节点名字:%s\n", json_object_get_string(temp));

        json_object_object_get_ex(proxy1,"local_port",&temp);
        printf("内端口:%s\n", json_object_get_string(temp));

        json_object_object_get_ex(proxy1,"remote_port",&temp);
        printf("外端口:%s\n", json_object_get_string(temp));
        json_object_put(proxy1);
        printf("\n");
    }
    json_object_put(j_GetProxiesList);
    json_object_put(temp);
}