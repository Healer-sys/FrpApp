#include "frpdownload.h"

int FrpDownload() {
    struct utsname sys_info;
    char command[200];
    char filename[50];

    // 获取系统信息
    if (uname(&sys_info) < 0) {
        perror("获取系统信息失败");
        return 1;
    }

    // 根据内核架构构建下载URL
    if (strcmp(sys_info.machine, "x86_64") == 0) {
        strncpy(filename, "frp_0.38.0_linux_amd64", sizeof(filename));
    } else if (strcmp(sys_info.machine, "aarch64") == 0) {
        strncpy(filename, "frp_0.38.0_linux_arm64", sizeof(filename));
    } else if (strcmp(sys_info.machine, "armv7l") == 0) {
        strncpy(filename, "frp_0.38.0_linux_arm", sizeof(filename));
    } else {
        printf("未找到适合该内核架构的FRP版本\n");
        return 1;
    }
    snprintf(command, sizeof(command), 
            "wget -q https://mirror.ghproxy.com/https://github.com/fatedier/frp/releases/download/v0.38.0/%s.tar.gz", filename);

    // 执行下载命令
    system(command);

    // 解压文件并存到frp和删除压缩包
    snprintf(command, sizeof(command), "tar -zxf %s.tar.gz && mv %s frp && rm %s.tar.gz", filename, filename, filename);
    system(command);

    return 0;
}
