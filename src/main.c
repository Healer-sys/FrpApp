#include "data.h"
#include "login.h"
#include "get.h"
#include "frp.h"
#include "tunnel.h"
#include "configdownload.h"
#include "Menu.h"
UserData_t *user;

int main(int argc, char **argv)
{	

    welcome();
	user = (UserData_t *)malloc(sizeof(UserData_t));

	//step 1 获取服务器
	if( !GetFrpServerList() ) {
		printf("err\n");
	}
	else {
		// free_frp_list();
	}


	//step 2 登陆并且获取隧道信息
	if(argc > 2) {
		user->username = strdup(argv[1]);
		user->password = strdup(argv[2]);
	}

	if( !login(user) ) {
		//printf("ok\n");
		free_frp_list();
		if (argc > 2) {
			free(user->password);
			free(user->username);
		}
		free(user);
		// free(user);
		return 0;
	}
	else {
	    get_tunnel(user);
	}
	
	//step 3 菜单显示
	printf("%d\n",argc);
	download_one( user);

	// Menu();
	
	// ShowAllList();
	// GetServerList(user);
	// ShowNode(user);
	// download(user, "香港-1号","me.ini");
	// download_all(user);


	free_tunnellist(user->tunnel);
	free_frp_list();
	free(user->frptoken);
	free(user->password);
	free(user->token);
	free(user->username);
	free(user);
	return 0;
}


__attribute__((constructor)) void init()
{
	system("clear");
}

// __attribute__((destructor)) void modfexit()
// {
// 	free_frp_list();
// 	printf("%s %s\n",user->username, user->password);
// 	free(user->frptoken);
// 	free(user->password);
// 	free(user->token);
// 	free(user->username);

// 	free(user->tunnel);
// 	free(user);
// 	printf("bey!\n");
// }
