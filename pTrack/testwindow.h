//#include <windows.h>
//
//BOOL createFreshWindow() {
//	HINSTANCE hinst;
//	HWND hwndMain;
//
//	// Create the main window. 
//
//	hwndMain = CreateWindowEx(
//		0,                      // no extended styles           
//		"MainWClass",           // class name                   
//		"Main Window",          // window name                  
//		WS_OVERLAPPEDWINDOW |   // overlapped window            
//		WS_HSCROLL |   // horizontal scroll bar        
//		WS_VSCROLL,    // vertical scroll bar          
//		CW_USEDEFAULT,          // default horizontal position  
//		CW_USEDEFAULT,          // default vertical position    
//		300,          // default width                
//		600,          // default height               
//		(HWND)NULL,            // no parent or owner window    
//		(HMENU)NULL,           // class menu used              
//		hinst,              // instance handle              
//		NULL);                  // no window creation data      
//
//	if (!hwndMain)
//		return FALSE;
//// Show the window using the flag specified by the program 
//// that started the application, and send the application 
//// a WM_PAINT message. 
//
//ShowWindow(hwndMain, SW_SHOWDEFAULT);
//UpdateWindow(hwndMain);
//}


/*use a thread
HANDLE thread = CreateThread(
	NULL, //default security attributes
	0,//use default stack size
	getCurrentWin,//thread function
	NULL,//argument to thread function
	0,//use default creation flags
	NULL// returns thread ID
	);
if (thread) {
	printf("\nDone!");
}
*/