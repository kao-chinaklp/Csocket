#pragma once
#include "CSocket.h"

using namespace csocket;

class ClientSock :
    public CSocket
{
public:
    ClientSock();
    ~ClientSock();
    void OnReceive();
};

