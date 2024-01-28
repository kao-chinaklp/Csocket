#include "CSocket.h"
#include <stdio.h>

using namespace csocket;

csocket::CSocket::CSocket(u_short PORT) {
	initialization();
	//���������Ϣ
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	//�����׽���
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	try {
		if (bind(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
			throw "Failed to create socket";
			WSACleanup();
		}
		//�����׽���Ϊ����״̬
		if (listen(s_server, SOMAXCONN) < 0) {
			throw "Failed to set listening status";
			WSACleanup();
		}
	}
	catch (std::string sockError) {
		char pause;
		printf("%s", sockError);
		scanf("%c", pause);
		exit(0);
	}
	printf("Success!");
    std::thread accpet(&CSocket::OnAccpet);
	accpet.join();
	std::thread recvive(&CSocket::OnReceive);
	recvive.join();
}

csocket::CSocket::~CSocket(){
}

void csocket::CSocket::OnAccpet(){
    int len = sizeof(SOCKADDR);
    SOCKET s_accept = accept(s_server, (SOCKADDR*)&accept_addr, &len);
    if (s_accept == SOCKET_ERROR) {
        printf("Failed to connect");
        WSACleanup();
		char pause;
		scanf("%c", pause);
		exit(0);
    }
}

void csocket::CSocket::OnClose(){
	//�ر��׽���
	closesocket(s_server);
	closesocket(s_accept);
	//�ͷ�DLL��Դ
	WSACleanup();
}

void csocket::CSocket::OnReceive(){
}

void csocket::CSocket::OnSend(){
}

void csocket::CSocket::initialization() {
	//��ʼ���׽��ֿ�
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	int err;
	try {
		err = WSAStartup(w_req, &wsadata);
		if (err != 0)throw "Failed to initialize socket library";
		if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
			throw "Socket library version number does not match";
			WSACleanup();
		}
	}
	catch (std::string sockError) {
		char pause;
		printf("%s", sockError);
		scanf("%c", pause);
		exit(0);
	}
}