//
// Created by Даниил Хазиев on 19.04.2023.
//

#ifndef GOODBYEDPI_PLUS_PROXYSERVER_H
#define GOODBYEDPI_PLUS_PROXYSERVER_H

#include "uv.h"
#include "iostream"
#include "string"

class ProxyServer {
protected:
    uv_tcp_t* tcpSock;
    uv_udp_t* udpSock;
    uv_pipe_t* uvPipe;
    uv_loop_t* loop;
    ProxyServer(auto fd, std::string &&host, uv_loop_t &&loop);

    std::vector<char> buffer;


public:

    virtual ~ProxyServer();
    static ProxyServer *createNew(auto fd, std::string &&host, uv_loop_t &&loop);
    virtual bool runServer();
    virtual bool stopServer();

};


#endif //GOODBYEDPI_PLUS_PROXYSERVER_H
