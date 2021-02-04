//#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "Processes.h"
#include <string>


#pragma comment (lib, "ws2_32.lib")

int __cdecl main(void)
{
	WSADATA wsaData;
	SOCKET ConnectSocket;
	sockaddr_in ServerAddr;
	int speaker, maxlen = 1024 * MAX_PATH;
	char* recvbuf = new char[1024];

	WORD ver = MAKEWORD(2, 2);

	WSAStartup(ver, &wsaData);

	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	ServerAddr.sin_family = AF_INET;
	InetPton(AF_INET, "127.0.0.1", &ServerAddr.sin_addr.s_addr);
	ServerAddr.sin_port = htons(8820);

	speaker = connect(ConnectSocket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));

	if (speaker == SOCKET_ERROR) {
		printf("connect failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	char* arr = new char[1024 * MAX_PATH];
	std::string temp;
	while (true)
	{
		ZeroMemory(recvbuf,1024);
		Processes winprocesses = Processes();

		std::string aProcesses = winprocesses.Get_processes();

		strcpy_s(arr, 1024* MAX_PATH, aProcesses.c_str());
		
		send(ConnectSocket, arr, strlen(arr), 0);

		std::cout << "data sent" << std::endl;

		speaker = recv(ConnectSocket, recvbuf, 1024, 0);
		temp = recvbuf;

		if (temp.substr(0,5) == "Stop")
		{
			printf("Connection closing...\n");
			closesocket(ConnectSocket);
			return 1;
		}
		else if (speaker > 0) {
			recvbuf[speaker] = 0;
			printf("Result: %s\n", (char*)recvbuf);
		}
		else if (speaker == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

	}

	delete[] arr;
	delete[] recvbuf;
	closesocket(ConnectSocket);

}