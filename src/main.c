#include "data.h"
#include "login.h"
#include "get.h"
#include "frp.h"
#include "Menu.h"

int main(int argc, char **argv)
{
	UserData_t *user = (UserData_t *)malloc(sizeof(UserData_t));
	FrpList_t frp[100];
	//chushihua
	
	for(int i = 0; i < 100; i++) {
		FrpList_t* frp_ = (FrpList_t*)malloc(sizeof(FrpList_t));
		frp[i] = *frp_;
	}
	
	if( !GetFrpServerList(frp) ) {
		printf("err\n");
	}

	for (int i = 0; i < 100; i++) {
		printf("%d:%d\n",i,frp[i].id);
        printf("%d %s %s %s %s %s\n", i, frp[i].name, frp[i].description, frp[i].ip, frp[i].hostname, frp[i].status);
	}

	// FrpList_t* frplist = InitFrpList();
	// if(frplist == NULL) {
	// 	printf("chushihuashibian\n");
	// }
	
	// welcome();

	// Menu();

	user->username = argv[1];
	user->password = argv[2];
	if( !login(user) ) {
		//printf("ok\n");
		return 0;
	}

	GetServerList(user);
	ShowNode(user);
	for(int i = 0; i < 100; i++) {
		free(&frp[i]);
	}
	free(user);
	return 0;
}
