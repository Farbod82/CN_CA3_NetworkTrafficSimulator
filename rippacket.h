#ifndef RIPPACKET_H
#define RIPPACKET_H

#include "packet.h"
#include <QHash>

class RipPacket : public Packet
{
public:
    RipPacket(std::string _dest_addr, std::string _source_addr);
    void addRoute(QHash<std::string, int> _routing);
private:
    QHash<std::string, int> routing;
};

#endif // RIPPACKET_H
