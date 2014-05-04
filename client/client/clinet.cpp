#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
using namespace std;

SOCKADDR_IN addr;
SOCKET sConnect;

class clinetData{
	public:
	int num;
	char msg[256];
};

int ClientThread(){
	clinetData data;
	char String[sizeof(data)] = {0};
	for(;; Sleep(10)){
		if(recv(sConnect, String, sizeof(data), NULL)){
			memcpy(&data, String, sizeof(data));
			cout << "Client number " << data.num << ": " << data.msg <<endl;
		}
	}
	return 0;
}

int main(){
	system("cls");
	int value = 0;
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2,1);
	value = WSAStartup(DllVersion, &wsaData);

	if(value != 0){
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	sConnect = socket(AF_INET, SOCK_STREAM, NULL);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port        = htons(1234);
	addr.sin_family      = AF_INET;
	cout << "Connect to Server? [ENTER]" <<endl;
	getchar();
	value = connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));

	if(value != 0){
		MessageBoxA(NULL, "Could not connect to server", "Error", MB_OK | MB_ICONERROR);
		main();
	}

	else{
		int num;
		char* cID = new char[64];
		ZeroMemory(cID, 64);
		recv(sConnect, cID, 64, NULL);
		num = atoi(cID);
		cout << "Connected" <<endl;
		cout << "You are Client No: " << num <<endl;
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) ClientThread, NULL, NULL, NULL);
		for(;; Sleep(10)){
			char* String = new char[256];
			ZeroMemory(String, 256);
			cin >> String;
			getchar();
			send(sConnect, String, 256, NULL);
		}
	}
	return 0;
}