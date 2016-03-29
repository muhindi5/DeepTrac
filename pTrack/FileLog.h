#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

HANDLE createMainlogFile(void);
BOOL writeToLog(HANDLE fileHandle,char message[]);

/*create log file in the current directory*/
HANDLE createMainlogFile() {
	HANDLE fHandle;
	char fileName[] = "PtrackLog.txt";
	fHandle = CreateFile(fileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (fHandle == INVALID_HANDLE_VALUE) {
		printf("Could not open file [ERROR]: %ld", GetLastError());
		exit(1);
	}
	return fHandle;
}


/*Log messages to file*/
BOOL writeToLog(HANDLE fileHandle, char messg[]) {
	DWORD bytesWritten;
	BOOL logged = FALSE;
		BOOL success = WriteFile(fileHandle, messg, strlen(messg), &bytesWritten, NULL);
		if (success != TRUE) {
			return logged;
		}
		logged = TRUE;
		return logged;
}
