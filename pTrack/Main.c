
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
//BOOL writeToLogF();
//VOID generateRandomStr(char holder[]);

int main(int argc, char *argv[]) {

	printf("Run at: %s\n--------------------------------------------\n", __TIME__);
	//getProcessInfo();
	//getProcesses();
	//createMyProcess();
	//EnumWindows(TestEnumWindowsProc, NULL);
	//setupWSA();
	//SOCKET socket = connectToServer();
	//getCurrentWin(socket);

	HANDLE handleToMainLog = createMainlogFile();
		writeToLog(handleToMainLog, "----------logging--------------");
		HHOOK hkHndDll = installHook(handleToMainLog);
		puts("Running...");	
		puts("Enter 1 to exit\n");
		while (getchar() == 1) {
			unHookDll(handleToMainLog, hkHndDll);
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
			writeToLog(logFileHandle, "Hook is Null!");
			printf("Error (NULL Hook): %ld\n", GetLastError());
			exit(1);
	 }

	hSysHook = SetWindowsHookEx(WH_CALLWNDPROC,hookProc,dllInstance,0);
	if (hSysHook == NULL) {
		writeToLog(logFileHandle, "Failed in installing Hook...Exiting!");
		printf("Error (Hook): %ld\n", GetLastError());
		exit(1);
	}
		writeToLog(logFileHandle, "Hook Installed Successfully!");
		CloseHandle(logFileHandle);
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

//BOOL writeToLogF() {
//	char fileName[10];
	char fName[] = "C:\\Users\\Kelli\\Documents\\GitHubVisualStudio\\ptrack_VS\\Debug\\";
//	char buffer[75]; //place concatenated string here
//	generateRandomStr(fileName);
//	snprintf(buffer, sizeof(buffer), "%s%s.txt", fName, fileName);
//	HANDLE fHandle =
//		CreateFile(buffer,
//			FILE_GENERIC_READ | FILE_GENERIC_WRITE,
//			FILE_SHARE_READ | FILE_SHARE_WRITE,
//			NULL, OPEN_ALWAYS,
//			FILE_ATTRIBUTE_NORMAL,
//			NULL);
//	if (fHandle == INVALID_HANDLE_VALUE) {
//		CreateFile("C:\\Users\\Kelli\\Documents\\GitHubVisualStudio\\ptrack_VS\\Debug\\error.log",
//			FILE_GENERIC_READ | FILE_GENERIC_WRITE,
//			FILE_SHARE_READ | FILE_SHARE_WRITE,
//			NULL, OPEN_ALWAYS,
//			FILE_ATTRIBUTE_NORMAL, NULL);
//	}
//	DWORD bytesWritten;
//	WriteFile(fHandle, buffer, strlen(buffer), &bytesWritten, NULL);
//	CloseHandle(fHandle);
//	return TRUE;
//}
//
//
///*Generate a log file name for the process */
//VOID generateRandomStr(char holder[]) {
//	size_t i = 0;
//	srand(time(NULL)); //seed random number generator using system clock seconds
//	while (i < 9) {
//		int x = (97 + rand() % 97);
//		if (x > 96 && x < 122) {
//			holder[i] = ((char)x);
//			//puts((char)x);
//			i++;
//		}
//	}
//	holder[i++] = '\0'; //terminate the string
//}
