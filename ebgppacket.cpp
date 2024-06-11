#include "ebgppacket.h"





EBPGPacket::EBPGPacket(std::string _dest_addr, std::string _source_addr):Packet(_dest_addr,_source_addr,"0.0.0.0","EBGP") {

}



void EBPGPacket::addRoute(std::vector<std::string> _prefixes){
    prefixes = _prefixes;
}

std::vector<std::string> EBPGPacket::getPrefixes(){
    return prefixes;
}




