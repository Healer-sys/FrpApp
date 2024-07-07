#include "data.h"
#include "login.h"
#include "get.h"
#include "frp.h"
#include "Menu.h"

int main(int argc, char **argv)
{
	UserData_t *user = (UserData_t *)malloc(sizeof(UserData_t));

	if( !GetFrpServerList() ) {
		printf("err\n");
	}
	else {
		// ShowList(GetServerForId(49));
	}

	// welcome();

	// Menu();

	user->username = argv[1];
	user->password = argv[2];
	if( !login(user) ) {
		//printf("ok\n");
		free_frp_list();
		free(user);
		return 0;
	}

	GetServerList(user);
	ShowNode(user);
	free_frp_list();
	free(user);
	return 0;
}
