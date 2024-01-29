#include "Csocket.h"

int RecvTask::Run(){
    char buffer[1024];
    while(true){
        int len=recv(socket, buffer, sizeof(buffer), 0);
        if(len==-1)throw std::runtime_error("recv failed");
        if(len==0)break;
        buffer[len]='\0';
        ptr->OnRecv(buffer, socket);
    }
    return 0;
}

Csocket::~Csocket(){
    Close();
}

void Csocket::Initialization(){
    #ifdef _WIN32
        socketVersion = MAKEWORD(2, 2);
        if(WSAStartup(socketVersion, &wsaData))
            throw std::runtime_error("WSAStartup failed");
    #endif
}

void Csocket::Create(){
    int af, typ, pro;
    if(protocol==Protocol::IPv4)af=AF_INET;
    else af=AF_INET6;
    if(type==CsocketType::TCP)typ=SOCK_STREAM, pro=IPPROTO_TCP;
    else typ=SOCK_DGRAM, pro=IPPROTO_UDP;
    #ifdef _WIN32
        socket=::socket(af, typ, pro);
    #else
        socket=::socket(af, typ, 0);
    #endif
    if(socket==-1ull)throw std::runtime_error("socket creation failed");
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
    this->backlog=backlog;
}

void Csocket::Connect(const char* IP, const ui port){
    if(protocol==Protocol::IPv4){
        addr.sin_family=AF_INET;
        addr.sin_port=htons(port);
        if(ServiceType::Server==serviceType)addr.sin_addr.s_addr=htonl(INADDR_ANY);
        else addr.sin_addr.s_addr=inet_addr(IP);
        if(::bind(socket, (SOCKADDR*)&addr, sizeof(addr))==-1)
            throw std::runtime_error("connect failed");
    }
    else{
        addr6.sin6_family=AF_INET6;
        addr6.sin6_port=htons(port);
        if(ServiceType::Server==serviceType)addr6.sin6_addr=in6addr_any;
        else inet_pton(AF_INET6, IP, &addr6.sin6_addr);
        if(::bind(socket, (SOCKADDR*)&addr6, sizeof(addr6))==-1)
            throw std::runtime_error("connect failed");
    }
    if(protocol==Protocol::IPv4){
        if(::connect(socket, (SOCKADDR*)&addr, sizeof(addr))==-1)
            throw std::runtime_error("connect failed");
    }
    else
        if(::connect(socket, (SOCKADDR*)&addr6, sizeof(addr6))==-1)
            throw std::runtime_error("connect failed");
}

void Csocket::Start(){
    if(ServiceType::Server==serviceType){
        ThreadPool=new CThreadPool(backlog);
        while(true){
            SOCKADDR_IN clientAddr;
            int len=sizeof(clientAddr);
            SOCKET clientSocket=accept(socket, (SOCKADDR*)&clientAddr, &len);
            if(clientSocket==-1ull)throw std::runtime_error("accept failed");
            RecvTask* task=new RecvTask();
            task->SetSocket(clientSocket);
            task->SetPtr(this);
            task->SetTaskName("RecvTask");
            ThreadPool->AddTask(task);
        }
    }
    else{
        RecvTask* task=new RecvTask();
        task->SetTaskName("RecvTask");
        ThreadPool->AddTask(task);
    }
}

void Csocket::Send(const char* Buffer, const ui len, const ui socket){
    if(::send(socket, Buffer, len, 0)==-1)
        throw std::runtime_error("send failed");
}

void Csocket::Close(){
    #ifdef _WIN32
        closesocket(socket);
        WSACleanup();
    #else
        close(socket);
    #endif
}