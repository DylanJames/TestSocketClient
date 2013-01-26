#include <iostream>
#include <stdio.h>
#include <WS2tcpip.h>
#include <WinSock2.h>		//You can't include both winsock.h & winsock2.h
#include <string>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "27015"
//char *argv[];				//Makes argv (below) work

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	int iResult;

	//Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	else cout << "Winsock has been initialized..." << endl;

	//Declare address struct with family info
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//Resolve the server address and port
	//Call getaddrinfo requesting IP address for server name passed
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	cout << "Resolved server address & port..." << endl;

	//Create socket object
	SOCKET ConnectSocket = INVALID_SOCKET;

	//Attempt to connect to the first address returned by the call to getaddrinfo
	ptr = result;
	//Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	//Check for errrors to ensure that the socket is a valid socket
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	cout << "No errors currently detected..." << endl;

	//Connect to server
	iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server.\n");
		WSACleanup();
		return 1;
	}

	char exit_char;									//Temporary for input
	cout << "\nPress any key and <enter> to exit \n";
	cin >> exit_char;								//Wait for key response before exiting

	return 0;
}