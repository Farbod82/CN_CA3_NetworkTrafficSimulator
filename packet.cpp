#include "packet.h"

Packet::Packet(std::string _dest_addr, std::string _source_addr,std::string _Type = "packet", IPVersion _ipv = IPV_4) {
    dest_addr = _dest_addr;
    source_addr = _source_addr;
    Type = _Type;
    ipv = _ipv;
}


void Packet::setNextHopPort(int _port){
    nextHopPort = _port;
}

std::string Packet::getBody(){
    return body;
}

void Packet::setBody(std::string _body){
    body = _body;
}


std::string Packet::getType(){
    return  Type;
}


std::string Packet::getSource(){
    return source_addr;
}


void Packet::addASNumber(int _AS){
    ASNumbers.push_back(_AS);
}

bool Packet::changeToIPV6(){
    if (ipv == IPV_6){
        return false;
    }
    packet = this;
    encapsulated = true;
    return true;
}

IPVersion Packet::getIpVersion(){
    return ipv;
}

void Packet::increaseDeliveryCycles(){
    deliveryCycles++;
}

void Packet::increaseBufferWaitingCycles(){
    bufferWaitingCycles++;
}
