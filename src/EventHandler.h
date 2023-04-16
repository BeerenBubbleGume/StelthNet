//
// Created by Даниил Хазиев on 28.03.2023.
//

#ifndef GOODBYEDPI_PLUS_EVENTHANDLER_H
#define GOODBYEDPI_PLUS_EVENTHANDLER_H
#include "utils/Driver.h"
#include <iostream>
#include "event2/event.h"

enum BLOCK_TIPE{
    BLOCK_RESPONSE,
    BLOCK_CONNLOSS,
    BLOCK_STRANGE_PACK,
    NO_BLOCK
};

class Block {
public:
    virtual void BlockEvent(BLOCK_TIPE type, evutil_socket_t fd) = 0;
};

class EventHandler : public PacketHandler {
protected:
    BLOCK_TIPE haveBlock;
    Block* block = nullptr;
private:
    event_base* eventBase;
    event_config* eventConfig;
    virtual void noResponse(timeval tv);
    virtual bool connectLoss(const u_char *string, unsigned len);
    virtual bool strangePackets(const u_char *string, unsigned len);


    static void heartBeat(evutil_socket_t fd, short event, void *arg);
    static void checkConnect(evutil_socket_t fd, short evetn, void *arg);
    static void onNoResponse(evutil_socket_t fd, short event, void *arg);

public:
    EventHandler();
    ~EventHandler() override;
    void packetHandler(const u_char *string, unsigned len) override;
};


#endif //GOODBYEDPI_PLUS_EVENTHANDLER_H
