#include "rippacket.h"

RipPacket::RipPacket(std::string _dest_addr, std::string _source_addr):Packet(_dest_addr,_source_addr,"0.0.0.0","RIP") {

}




void RipPacket::addRoute(QHash<std::string, int> _routing){
    routing = _routing;
}

QHash<std::string, int> RipPacket::getRoute(){
    return routing;
}




