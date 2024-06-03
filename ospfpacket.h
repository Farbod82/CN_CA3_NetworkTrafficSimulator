#ifndef OSPFPACKET_H
#define OSPFPACKET_H

#include "packet.h"
#include <QHash>
#include <QObject>

typedef QHash<std::string, int> Link;

class OspfPacket : public Packet
{
public:
    OspfPacket(std::string _source_addr, Link _link, QObject *parent);
    void decreaseTTL();
    int getTTL();
    int getSequence();
    Link getLinks();

private:
    int sequence = 0;
    int ttl = 10;
    Link links;
public slots:
    void incSequence();
};

#endif // OSPFPACKET_H
