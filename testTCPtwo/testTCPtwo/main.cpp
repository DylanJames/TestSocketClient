#include <iostream>
#include <stdio.h>
#include <WS2tcpip.h>
#include <WinSock2.h>		//You can't include both winsock.h & winsock2.h
#include <string>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

SOCKET s; //Socket handle
bool ConnectToHost (int, char*);
void CloseConnection ();

//CONNECTTOHOST – Connects to a remote host
int main(int argc, char *argv[]) {
	ConnectToHost(21, "50.23.218.100");
	CloseConnection();

	char exit_char;									//Temporary for input
	cout << "\nPress any key and <enter> to exit \n";
	cin >> exit_char;								//Wait for key response before exiting

	return 0;
}
bool ConnectToHost(int PortNo, char* IPAddress)
{
    //Start up Winsock…
    WSADATA wsadata;

    int error = WSAStartup(0x0202, &wsadata);

    //Did something happen?
    if (error) {
		cout << "Error." << endl;
        return false;
	}

    //Did we get the right Winsock version?
    if (wsadata.wVersion != 0x0202)
    {
        WSACleanup(); //Clean up Winsock
		cout << "Winsock Error." << endl;
        return false;
    }

    //Fill out the information needed to initialize a socket…
    SOCKADDR_IN target; //Socket address information

    target.sin_family = AF_INET; // address family Internet
    target.sin_port = htons (PortNo); //Port to connect on
    target.sin_addr.s_addr = inet_addr (IPAddress); //Target IP

    s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
    if (s == INVALID_SOCKET)
    {
		cout << "Can't create socket." << endl;
        return false; //Couldn't create the socket
    }  

    //Try connecting...

    if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
    {
		cout << "Can't connect\n.";
        return false; //Couldn't connect
    }
    else {
		cout << "Success!" << endl;		//Add function to report back port number for testing
        return true; //Success
	}
}

//CLOSECONNECTION – shuts down the socket and closes any connection on it
void CloseConnection ()
{
    //Close the socket if it exists
    if (s)
        closesocket(s);

    WSACleanup(); //Clean up Winsock
}