//
// Created by Даниил Хазиев on 28.03.2023.
//

#ifndef GOODBYEDPI_PLUS_DRIVER_H
#define GOODBYEDPI_PLUS_DRIVER_H

#include <vector>
#include "pcap/pcap.h"
#include <iostream>

class Driver;

class PacketHandler {
protected:
    friend class Driver;
public:
    virtual void packetHandler(const u_char *string, unsigned len) = 0;
    PacketHandler() = default;
    virtual ~PacketHandler() = default;
};


class Driver {
private:

    PacketHandler* packetHandler;

    static void packetHandlerWrapper(u_char* user, const struct pcap_pkthdr* header, const u_char* packet) {
        PacketHandler* handler = (PacketHandler*)user;
        handler->packetHandler(packet, header->len);
    }
protected:
    pcap_t* handler;
    pcap_if** nextDev;
    bpf_program fileDump;
    bpf_u_int32 net, mask;
    char errBuff[PCAP_ERRBUF_SIZE];
    unsigned device;
    std::string filter;
    explicit Driver(PacketHandler* packetHandler, const char* filter = nullptr);
public:
    ~Driver() {
        stop();
        if(packetHandler){
            delete packetHandler;
            packetHandler = nullptr;
        }
    };

    static pcap_t* getPcapHandler(Driver&& dr) {return dr.handler;}
    static int getSocket(Driver&& dr) {return pcap_get_selectable_fd(dr.handler);}
    static Driver* createNew(class PacketHandler* packetHandler, const char* filter = nullptr);
    virtual void run(class PacketHandler* packetHandler);
    virtual void stop();
};
#endif //GOODBYEDPI_PLUS_DRIVER_H