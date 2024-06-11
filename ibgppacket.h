#ifndef IBPGPACKET_H
#define IBPGPACKET_H

#include "packet.h"
#include <QHash>

class IBPGPacket : public Packet
{
public:
    IBPGPacket(std::string _dest_addr, std::string _source_addr);
    void setPacket(std::shared_ptr<Packet> _packet);
    void setfinalDest(std::string _finalDest);
    void setType(std::string _type);
    std::string getFinalDest();
private:
    std::string finalDest;
};

#endif
