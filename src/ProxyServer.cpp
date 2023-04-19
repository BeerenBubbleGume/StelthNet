//
// Created by Даниил Хазиев on 19.04.2023.
//

#include "ProxyServer.h"

ProxyServer::ProxyServer(auto fd, std::string &&host, uv_loop_t &&loop) {

}

ProxyServer::~ProxyServer() {

}

ProxyServer *ProxyServer::createNew(auto fd, std::string &&host, uv_loop_t &&loop) {
    return new ProxyServer(fd, host, loop);
}

bool ProxyServer::stopServer() {
    return false;
}

bool ProxyServer::runServer() {
    return false;
}
