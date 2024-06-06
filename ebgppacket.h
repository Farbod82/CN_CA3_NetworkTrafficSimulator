#ifndef EBPGPACKET_H
#define EBPGPACKET_H

#include "packet.h"
#include <QHash>

class EBPGPacket : public Packet
{
public:
    EBPGPacket(std::string _dest_addr, std::string _source_addr);
    void addRoute(std::vector<std::string> _prefixes);
    std::vector<std::string> getPrefixes();
private:
    std::vector<std::string> prefixes;


};

#endif
