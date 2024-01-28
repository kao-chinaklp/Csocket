#ifndef CSOCKET_H_
#define CSOCKET_H_

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include "Thread.h"

enum class CsocketType{TCP, UDP};
enum class Protocol{IPv4, IPv6};
enum class ServiceType{Client, Server};

class Csocket{
    public:
        Csocket(CsocketType type, Protocol protocol, ServiceType serviceType):
            type(type), protocol(protocol), serviceType(serviceType){
            #ifdef _WIN32
                Initialization();
            #endif
        }
        ~Csocket();

        void Initialization();
        void Create();
        void Bind(const ui port, const char* IP=nullptr);
        void Listen(const ui backlog=5);
        void Close();

    private:
        CsocketType type;
        Protocol protocol;
        ServiceType serviceType;
        #ifdef _WIN32
            SOCKET socket;
            SOCKADDR_IN addr;
            SOCKADDR_IN6 addr6;
            WORD socketVersion;
            WSADATA wsaData;
        #else
            int socket;
        #endif

};

#endif // CSOCKET_H_