#include "ospfpacket.h"

OspfPacket::OspfPacket(std::string _source_addr, Link _link, QObject *parent = nullptr):Packet("broadcast",_source_addr,"OSPF") {
    links = _link;
}

void OspfPacket::decreaseTTL(){
    ttl--;
}

int OspfPacket::getTTL(){
    return ttl;
}

int OspfPacket::getSequence(){
    return sequence;
}

void OspfPacket::incSequence(){
    sequence++;
}

Link OspfPacket::getLinks(){
    return links;
}
