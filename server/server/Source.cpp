#pragma comment(lib, "Ws2_32.lib")

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
using namespace std;

SOCKADDR_IN address;
SOCKET sockListen;
SOCKET sockConnect;
SOCKET* clients;

int addressLen = sizeof(address);
int countOfClients = 0;

class clientData{
	public:
		int num;
		char msg[256];	//from 0 to 255
};

int serverManagment(int num){
	clientData data;
	char* receive = new char[256];
	ZeroMemory(receive, 256);
	char* Send = new char[sizeof(clientData)];
	ZeroMemory(Send, sizeof(clientData));	
	for(;; Sleep(10)){
		if(recv(clients[num], receive, 256, NULL)){
			data.num = num;
			memcpy(data.msg, receive, 256);		
			memcpy(Send, &data, sizeof(clientData));
			for(int a = 0; a != countOfClients; a++){
				if(clients[a] == clients[num]){}
				else{
					send(clients[a], Send, sizeof(clientData), NULL);
				}
			}
			ZeroMemory(receive, 256);
		}
	}
	return 0;
}

int Init_Socket(){
	int value = 0;
	WSAData wsaData;	//the struct contains information about the windows socket implementation
	WORD DllVersion = MAKEWORD(2,1);
	value = WSAStartup(DllVersion, &wsaData);
	return value;
}

int main(){
	int value = 0;
	value = Init_Socket();
	if(value != 0){
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	clients = (SOCKET*)calloc(64, sizeof(SOCKET));	//allocates an array in memory with elements initialized to zero
	sockListen = socket(AF_INET, SOCK_STREAM, NULL);
	sockConnect = socket(AF_INET, SOCK_STREAM, NULL);

	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port        = htons(1234);
	address.sin_family      = AF_INET;

	bind(sockListen, (SOCKADDR*)&address, sizeof(address));		//binds arguments to a callable object
	listen(sockListen, 64);
	for(;; Sleep(50)){
		if(sockConnect = accept(sockListen, (SOCKADDR*)&address, &addressLen)){
			clients[countOfClients] = sockConnect;
			char* num = new char[64];
			ZeroMemory(num, sizeof(num));	
			_itoa(countOfClients, num, 10);		//converts az integer to a string	
			send(clients[countOfClients], num, sizeof(num), NULL);
			countOfClients ++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) serverManagment, (LPVOID)(countOfClients - 1), NULL, NULL);
		}
	}
	return 0;
}