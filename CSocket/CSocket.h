#pragma once

#include <winsock2.h>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib")  

/*
 * ͻ��������д�µģ�
 */

namespace csocket {
	class CSocket {
	private:
		//���������׽��֣����������׽���
		SOCKET s_server;
		SOCKET s_accept;
		//����˵�ַ�ͻ��˵�ַ
		SOCKADDR_IN server_addr;
		SOCKADDR_IN accept_addr;

    public:
        CSocket(u_short PORT);
		~CSocket();
        virtual void OnAccpet();
        virtual void OnClose();
        virtual void OnReceive();
		virtual void OnSend();
		void initialization();
	};
}