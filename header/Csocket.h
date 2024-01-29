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

class Csocket;

class RecvTask:public CTask{
    public:
        RecvTask()=default;
        RecvTask(String &taskName):CTask(taskName){}
        void SetSocket(int socket){this->socket=socket;}
        void SetPtr(Csocket* ptr){this->ptr=ptr;}
        int Run()override;

    protected:
        Csocket* ptr;
        #ifdef _WIN32
            SOCKET socket;
        #else
            int socket;
        #endif
};

class Csocket{
    public:
        Csocket(CsocketType type, Protocol protocol, ServiceType serviceType, const ui port, const char* IP=nullptr):
            type(type), protocol(protocol), serviceType(serviceType){
            #ifdef _WIN32
                Initialization();
            #endif
            if(ServiceType::Server==serviceType){
                Create();
                Bind(port, IP);
                Listen();
            }
            else Connect(IP, port);
        }
        ~Csocket();

        void Initialization();
        void Create();
        void Bind(const ui port, const char* IP=nullptr);
        void Listen(const ui backlog=5);
        void Connect(const char* IP, const ui port);
        void Start();
        void Send(const char* Buffer, const ui len, const ui socket=0);
        void Close();

    public:
        virtual void OnRecv(const char* Buffer, ui socket)=0;

    private:
        ui backlog;
        CThreadPool* ThreadPool;

        Deque<String>BufferList;
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
            sockaddr_in addr;
            sockaddr_in6 addr6;
        #endif

};

#endif // CSOCKET_H_