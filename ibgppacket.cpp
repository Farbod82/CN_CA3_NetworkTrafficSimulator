#include "ibgppacket.h"





IBPGPacket::IBPGPacket(std::string _dest_addr, std::string _source_addr):Packet(_dest_addr,_source_addr,"IBGP") {

}

void IBPGPacket::setfinalDest(std::string _finalDest){
    finalDest = _finalDest;
}

std::string IBPGPacket::getFinalDest(){
    return finalDest;
}



