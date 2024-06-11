#include "ibgppacket.h"





IBPGPacket::IBPGPacket(std::string _dest_addr, std::string _source_addr):Packet(_dest_addr,_source_addr,"0.0.0.0","IBGP") {

}

void IBPGPacket::setfinalDest(std::string _finalDest){
    finalDest = _finalDest;
}

std::string IBPGPacket::getFinalDest(){
    return finalDest;
}

void IBPGPacket::setType(std::string _type){
    Type = _type;
}


