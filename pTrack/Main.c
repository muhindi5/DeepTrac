
#include <stdio.h>
#include <stdlib.h>
//#include "modProcessTrack.h"
#include "Tests.h"
//#include "TestWindow.h"
//#include "Dispatch.h"
//#include "Messanger.h"

int main(int argc, char *argv[]) {

	char *winText = "Linux+Wins";
	printf("Run at: %s\n--------------------------------------------\n", __TIME__);
	//getProcessInfo();
	//getProcesses();
	//createMyProcess();
	//EnumWindows(TestEnumWindowsProc, NULL);
	setupWSA();
	SOCKET socket = connectToServer();
	getCurrentWin(socket);
	//cleanup(socket);
	return 0;
}

