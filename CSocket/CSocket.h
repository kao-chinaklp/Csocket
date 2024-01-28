#pragma once

#include <winsock2.h>
#include <thread>
#include <string>

#pragma comment(lib, "ws2_32.lib")  

/*
 * 突发奇想想写下的（
 */

namespace csocket {
	class CSocket {
	private:
		//定义服务端套接字，接受请求套接字
		SOCKET s_server;
		SOCKET s_accept;
		//服务端地址客户端地址
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