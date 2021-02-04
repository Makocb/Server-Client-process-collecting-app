#include <iostream>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>


#pragma comment (lib, "ws2_32.lib")

void main()
{
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		std::cerr << "Cant Innitialize winsock! Quitting!"<<std::endl;
		return;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

	if (listening == INVALID_SOCKET)
	{
		std::cerr << "Cant Innitialize winsock! Quitting!" << std::endl;
		return;
	}

	sockaddr_in	hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(8820);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);
	
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		std::cout << host << "connected on port" << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << "connected on port " <<
			ntohs(client.sin_port) << std::endl;
	}

	closesocket(listening);

	char buf[1024 * MAX_PATH];
	int counter=0;
	std::ofstream Processes("Processes.txt");
	//Processes.open("Processes.txt");

	while (true)
	{

		ZeroMemory(buf, 1024 * MAX_PATH);

		int bytesReceived = recv(clientSocket, buf, 1024 * MAX_PATH, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			std::cerr << "Error in recv(). Quitting" << std::endl;
			break;
		}

		if (bytesReceived == 0)
		{
			std::cout << "Client disconnected" << std::endl;
			break;
		}


		counter++;
		if (counter != 5)
		{
			Processes << buf<<std::endl;
			send(clientSocket, "Data received", sizeof("Data received"), 0);
		}
		else
		{
			send(clientSocket, "Stop", sizeof("Stop"), 0);
			break;
		}
		
	}

	closesocket(clientSocket);
	Processes.close();

	WSACleanup();

}