#include "Csocket.h"

#include <cstring>

Csocket::~Csocket(){
    Close();
}

void Csocket::Initialization(){
    #ifdef _WIN32
        socketVersion = MAKEWORD(2, 2);
        WSAStartup(socketVersion, &wsaData);
    #endif
}

void Csocket::Create(){
    int af, typ, pro;
    if(protocol==Protocol::IPv4)af=AF_INET;
    else af=AF_INET6;
    if(type==CsocketType::TCP)typ=SOCK_STREAM;
    else typ=SOCK_DGRAM;
    #ifdef _WIN32
        if(protocol==Protocol::IPv4)pro=IPPROTO_TCP;
        else pro=IPPROTO_UDP;
        socket=::socket(af, typ, pro);
    #else
        socket=::socket(af, typ, 0);
    #endif
    if(socket==-1)throw std::runtime_error("socket creation failed");
}

void Csocket::Bind(const ui port, const char* IP){
    if(protocol==Protocol::IPv4){
        addr.sin_family=AF_INET;
        addr.sin_port=htons(port);
        if(ServiceType::Server==serviceType)addr.sin_addr.s_addr=htonl(INADDR_ANY);
        else addr.sin_addr.s_addr=inet_addr(IP);
        if(::bind(socket, (SOCKADDR*)&addr, sizeof(addr))==-1)
            throw std::runtime_error("bind failed");
    }
    else{
        addr6.sin6_family=AF_INET6;
        addr6.sin6_port=htons(port);
        if(ServiceType::Server==serviceType)addr6.sin6_addr=in6addr_any;
        else inet_pton(AF_INET6, IP, &addr6.sin6_addr);
        if(::bind(socket, (SOCKADDR*)&addr6, sizeof(addr6))==-1)
            throw std::runtime_error("bind failed");
    }
}

void Csocket::Listen(const ui backlog){
    if(::listen(socket, backlog)==-1){
        Close();
        throw std::runtime_error("listen failed");
    }
}

void Csocket::Close(){
    #ifdef _WIN32
        closesocket(socket);
        WSACleanup();
    #else
        close(socket);
    #endif
}