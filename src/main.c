#include "data.h"
#include "login.h"
#include "get.h"
#include "Menu.h"

int main(int argc, char **argv)
{
	UserData_t *user = (UserData_t *)malloc(sizeof(UserData_t));
	
	// welcome();

	// Menu();

	user->username = argv[1];
	user->password = argv[2];
	if(strcmp(login(user),"err") == 0) {
		//printf("ok\n");
		return 0;
	}


	GetServerList(user->username, user->token, user->frptoken);
	ShowNode(user->username, user->token);
	free(user);
	return 0;
}
