#ifndef DISPATCH_H
#define DISPATCH_H
#define WINSOCK_DEPRECATED_NO_WARNINGS

/*To prevent windows.h from including the WSA 1.1 automatically (would cause confilict with winsock2.h), 
the macro below is defined. It prevents winsock.h from being included by the windows.h header. */
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif

#define PORT "8080"
//includes winsock.h automatically i.e WSA 1.1
//automatically includes core elements from windows.h
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib") 
#pragma warning(disable : 4996)

//tell linker that winsock2 library is needed.

	SOCKET sockDescriptor = INVALID_SOCKET;
	WSADATA wsaData;
	char *requestStr, server_reply[2000]; 
	int receive_size = 256;

//start/iniitalize the winsock library
int initWSA(){
	DWORD dwErr;
	WORD version = MAKEWORD(2, 2); //create a 16 bit word using high-order and low-order bits
	dwErr = WSAStartup(version,&wsaData);
	if (dwErr != 0) {
		printf("WSA startup failed (Error Code): %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	/*check for correct version of WSA*/
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		printf("Version of WSA not correct!\nError: %d", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	puts("WSA Initialized\n");
	return 0;
}

/** create socket and connect*/
int  socketConnect(void) {
	
	//protocol address for remote socket
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&(serverAddr.sin_addr)); //insert IP address in network byte order

	//using hostent structure to get address
	serverAddr.sin_port = htons(8080); //glassfish server
	/*struct hostent *host;
	host = gethostbyname("localhost");
	serverAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr_list[0]);*/

	//create local socket
	sockDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockDescriptor < 0) {
		printf("Error creating socket: (%ld)", WSAGetLastError());
		getchar();
		exit(1);
	}

	//connect to server
	if (connect(sockDescriptor, (struct sockaddr *)(&serverAddr), sizeof(serverAddr)) < 0) {
		printf("Error connecting to socket\nError: %d", WSAGetLastError());
		getchar();
		exit(1);
	}
		puts("Connected to WildFly server");

		requestStr = "GET /ptrack/trec?title=From C with Love HTTP/1.1\r\n"
			"Host: localhost\r\n"
			"Accept: */*\r\n"
			"\r\n";
		if (send(sockDescriptor, requestStr, strlen(requestStr), 0) < 0) {
			puts("send failed");
			return 1;
		}
		printf("data sent: %s\n",requestStr);

		//receive reply from server. recv() returns num of bytes received
		if (receive_size = recv(sockDescriptor, server_reply, 2000, 0) == SOCKET_ERROR) {
			puts("recv() failed\n");
			return 1;
		}
		puts("received data\n");
		//server_reply[receive_size] = '\0'; //terminate string
		printf("\nReceived %d bytes\n",receive_size);

		//closesocket(sockDescriptor);
		//WSACleanup();
		return sockDescriptor; //return  a socket connected to server IP and port 8080
}



//void postData(SOCKET sock) {
//	char httpStr[2048];
//	char * host = "127.0.0.1:8080";
//	char  data[] = "?title=How to Get Moving - Mozilla Firefox";
//	//craft HTML query string and put in buffer (httpStr)
//	sprintf_s(httpStr, sizeof(httpStr), "GET /ptrack/trec?title=How to Get Moving - Mozilla Firefox HTTP/1.1\r\n"
//		"Host: %s\r\n"
//		//"Content-type: application/x-www-form-url-encoded\r\n"
//		"Accept-charset: utf-8\r\n"
//		"Content-length: %d\r\n\r\n", host,sizeof(httpStr));
//	puts(httpStr);
//
//	int bytesSent = send(sock, httpStr, strlen(httpStr), 0);
//	if (bytesSent == SOCKET_ERROR) {
//		printf("Error sending data: %ld",WSAGetLastError());
//	}
//	printf("Total Bytes sent: %d bytes\n", bytesSent);
//	/*pull down the tents ;)*/
//	closesocket(sock);
//	WSACleanup();
//}
#endif /*DISPATCH_H*/
