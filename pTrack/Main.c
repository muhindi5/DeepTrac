
#include <stdio.h>
#include <stdlib.h>
//#include "modProcessTrack.h"
#include "Tests.h"
#include "WinHook.h"
#include "FileLog.h"
//#include "TestWindow.h"
//#include "Dispatch.h"
//#include "Messanger.h"

//function prototypes
HHOOK installHook(HANDLE);
BOOL unHookDll(HANDLE, HHOOK);

int main(int argc, char *argv[]) {

	printf("Run at: %s\n--------------------------------------------\n", __TIME__);
	//getProcessInfo();
	//getProcesses();
	//createMyProcess();
	//EnumWindows(TestEnumWindowsProc, NULL);
	//setupWSA();
	//SOCKET socket = connectToServer();
	//getCurrentWin(socket);
		HANDLE h = createLogFile();
		writeToLog(h, "----------Logging--------------");
		HHOOK hHookHnd = installHook(h);
		puts("Running...");	
		puts("Enter 1 to exit\n");
		Sleep(50000);//pause for 2 minutes, then exit
		if (getchar() == 1) {
			unHookDll(h, hHookHnd);
			exit(0);
		}
	return 0;
}

/*Install hook procedure into the hook chain*/
HHOOK installHook(HANDLE logFileHandle) {
	
	writeToLog(logFileHandle, "Inside InstallHook:");
	 //HMODULE dllInstance; //handle to the DLL module containing the functions.
	 HHOOK hSysHook; //handle to installed system hook, use in unhooking
	 HOOKPROC hookProc; //

	HMODULE dllInstance = LoadLibrary("PtHookDll.dll");
	if (dllInstance == NULL) {
		writeToLog(logFileHandle, "Failed to load DLL...Exiting!");
		printf("Error:(LoadLib) %ld\n", GetLastError());
		exit(1);
	}

	writeToLog(logFileHandle, "DLL Loaded!");
	 hookProc = (HOOKPROC)GetProcAddress(dllInstance,"_GetMsgProc@12");
	 if (hookProc == NULL) {
			printf("Error (NULL Hook): %ld\n", GetLastError());
			exit(1);
	 }

	hSysHook = SetWindowsHookEx(WH_GETMESSAGE,hookProc,dllInstance,0);
	if (hSysHook == NULL) {
		writeToLog(logFileHandle, "Failed in installing Hook...Exiting!");
		printf("Error (Hook): %ld\n", GetLastError());
		exit(1);
	}
		writeToLog(logFileHandle, "Hook Installed Successfully!");
		return hSysHook;
}

/*Remove hook procedure installed in hook chain*/
BOOL unHookDll(HANDLE logFileHandle, HHOOK handleToDll) {
	BOOL unloaded = FALSE;
		unloaded = UnhookWindowsHookEx(handleToDll);
		//failed in hook proc uninstall
		if (unloaded = 0) {
			writeToLog(logFileHandle, " [Error:] Failed to unhook procedure from hookchain");
			exit(1);
		return unloaded;
	}
}