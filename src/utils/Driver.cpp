//
// Created by Даниил Хазиев on 28.03.2023.
//

#include "Driver.h"

Driver::Driver(PacketHandler* packetHandler, const char* filter) : filter(filter) {
    device = pcap_findalldevs(nextDev,errBuff);
    if(device == -1) {
        printf("Could not find any network device! \nERROR: %s", errBuff);
        exit(EXIT_FAILURE);
    }
    run(packetHandler);
}

void Driver::run(PacketHandler* packetHandler) {
    for(int i = 0; i < pcap_findalldevs(nextDev,errBuff); i++){
        if(pcap_lookupnet(nextDev[i]->name, &net, &mask, errBuff) == -1){
            printf("Could not get network device information!\nERROR: %s", errBuff);
            exit(EXIT_FAILURE);
        }
        if((handler = pcap_open_live(nextDev[i]->name, BUFSIZ, 1, 1000, errBuff)) == nullptr) {
            printf("Could not create handler!\nERROR: %s", errBuff);
            exit(EXIT_FAILURE);
        }
    }

    if(!filter.empty()){
        if (pcap_compile(handler, &fileDump, filter.c_str(), 0, net) == -1) {
            printf("Could not compile filter!\nERROR: %s", errBuff);
            exit(EXIT_FAILURE);
        }
        if (pcap_setfilter(handler, &fileDump) == -1) {
            printf("Could not setup filter!\nERROR: %s", errBuff);
            exit(EXIT_FAILURE);
        }
    }
    pcap_loop(handler, -1, packetHandlerWrapper, (u_char*)packetHandler);
}

void Driver::stop() {
    pcap_close(this->handler);
    if(!filter.empty()){
        std::destroy(filter.begin(), filter.end());
    }
}

Driver *Driver::createNew(PacketHandler* packetHandler, const char *filter) {
    return new Driver(packetHandler, filter);
}
