//
// Created by Даниил Хазиев on 28.03.2023.
//

#include <net/ethernet.h>
#include <netinet/tcp.h>
#include "EventHandler.h"
#include "pcap.h"

EventHandler::~EventHandler() {
    if(eventBase){
        event_base_free(eventBase);
    }
    if(eventConfig){
        event_config_free(eventConfig);
    }
}

EventHandler::EventHandler() {
    eventConfig = event_config_new();
    event_config_set_flag(eventConfig, EVENT_BASE_FLAG_IGNORE_ENV);
    eventBase = event_base_new_with_config(eventConfig);
    if(!eventBase)
        throw std::runtime_error("Failed to create event base");
    haveBlock = NO_BLOCK;
}

void EventHandler::packetHandler(const u_char *string, unsigned int len) {
    timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    noResponse(tv);
    haveBlock = strangePackets(string, len) ? BLOCK_STRANGE_PACK : NO_BLOCK;
}

void EventHandler::noResponse(timeval tv) {
    event* ev = event_new(eventBase, -1, EV_TIMEOUT, onNoResponse, block);
    event_add(ev, &tv);
    event_base_dispatch(eventBase);
}

bool EventHandler::strangePackets(const u_char *string, unsigned int len) {
    ether_header* etherHeader = (ether_header*)string;
    const u_char* ipHeader = string + sizeof(ether_header);
    int version = (*ipHeader >> 4);
    if(version == 4){
        auto headerLen = (*ipHeader & 0x0F) * 4;
        const u_char* tcpHeader = string + sizeof(ether_header) + headerLen;
        u_short destPort = ntohs(*(u_short*)(tcpHeader + 2));
        if (destPort == 80){
            const u_char* httpHeader;
            httpHeader = string + sizeof(struct ether_header) + headerLen + sizeof(tcphdr);
            if (strncmp((const char*)httpHeader, "GET", 3) == 0){
                const u_char* url;
                url = httpHeader + 4;
                std::cout << "HTTP request for " << url << std::endl;
            }
        }
        if(len < sizeof(ether_header) + sizeof(ipHeader) + sizeof(tcphdr)){
            printf("Malformed packet: too short\n");
            return true;
        }
        if (ntohs(etherHeader->ether_type) != ETHERTYPE_IP){
            printf("Unexpected protocol in Ethernet frame\n");
            return true;
        }
        if ((*ipHeader >> 4) != 4){
            printf("Unexpected IP version\n");
            return true;
        }
        if (*(ipHeader + 9) != IPPROTO_TCP){
            printf("Unexpected protocol in IP header\n");
            return true;
        }
        if (len != ntohs(*(u_short*)(ipHeader + 2))){
            printf("Malformed packet: incorrect length in IP header\n");
            return true;
        }
    }
    return false;
}

void EventHandler::onNoResponse(int fd, short event, void *arg) {
    if(arg){
        if (!EventHandler::heartBeat(fd)){
            Block* block = (Block*)arg;
            block->BlockEvent(BLOCK_RESPONSE, fd);
        }
    }
}

bool EventHandler::heartBeat(int fd) {
    auto msg = "\n";
    if(auto ret = send(fd, (void*)&msg, strlen(msg),0) < 0) return false;
    return true;
}
