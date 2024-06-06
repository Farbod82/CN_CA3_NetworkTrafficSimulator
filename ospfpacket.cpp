#include "ospfpacket.h"

OspfPacket::OspfPacket(std::string _source_addr, Link* _link):Packet("broadcast",_source_addr,"OSPF") {
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
    return *links;
}

std::shared_ptr<OspfPacket> OspfPacket::copy(){
    std::shared_ptr<OspfPacket> cpy = std::make_shared<OspfPacket>(source_addr, links);
    cpy->ttl = ttl;
    cpy->sequence = sequence;
    return cpy;
}

