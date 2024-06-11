#include "router.h"
#include "node.h"
#include "rippacket.h"
#include "ebgppacket.h"

#include <iostream>
#include <bits/shared_ptr.h>
#include <string>

#include "routingtable.h"
#include "ibgppacket.h"


Router::Router(int _id, std::string _ip, int _AS,std::string _mask, QObject *parent)
    : Node(_id,parent)
{
    isBorder = false;
    AS = _AS;
    ip = _ip;
    id = _id;
    mask = _mask;
    routingProtocl = RIP;
    distanceVector[ip] = 0;
    shoretestPathPorts[ip] = 0;
    for(int i =0; i < NUMBER_OF_PORTS; i++){
        Buffer* buffer = new Buffer(i);
        ports.push_back(buffer);
        neighbors[i] = "";
    }
    routingTable = new RoutingTable(ip);

}



void Router::setAsBorder(){
    isBorder = true;
}


bool Router::DoesBGPTableContain(std::string prefix) {
    return BGPTable.contains(prefix);
}


void Router::processPacketsOnSignal(){

    for (int i = 0; i < NUMBER_OF_PORTS; ++i) {
        ports[i]->increaseDeliveryCycles();
    }

    std::shared_ptr<Packet> packet = ports[servingPortBuffer]->getFirstPacket();
    if (packet.get() != nullptr){
        packet->addPath(ip);
        if (packet->getInitialASNumber() == AS){
            processPackets(packet, servingPortBuffer);
        }
        if (packet->getInitialASNumber() != AS && isBorder){
            processBGP(packet,servingPortBuffer);
        }

    }
    for (int i = 0; i < NUMBER_OF_PORTS; ++i) {
        ports[i]->incWaitingCycles();
        ports[i]->sendPacket();
    }

    servingPortBuffer +=1;
    servingPortBuffer %= NUMBER_OF_PORTS;

}
void Router::processBGP(std::shared_ptr<Packet> packet,int inputPort){
    if(packet->getType().compare("EBGP") == 0){
        auto ebgp = std::dynamic_pointer_cast<EBPGPacket>(packet);
        std::vector<std::string> prefixes = ebgp->getPrefixes();
        bool updated = false;
        for(int i =0 ; i < prefixes.size(); i++){
            if (!DoesBGPTableContain(prefixes[i])){
                updated = true;
                BGPTable[prefixes[i]] = packet->getSource();
            }
        }
        if (updated){
            // startIbgp();
        }
    }
    else if (packet->getType().compare("BGP") == 0){
        packet->addASNumber(AS);
        forwardPacket(packet,inputPort);
    }
}

void Router::sendToOtherAS(std::shared_ptr<Packet> packet){
    packet->setPacketDestination(packet->getFinalDest());
    // packet->addASNumber(AS);
    ports[4]->addToOutBuffer(packet);
}

void Router::processPackets(std::shared_ptr<Packet> packet,int inputPort){

    // std::cout << "router " << id << " got packet " << packet.get()->getBody() << " on port " << inputPort << std::endl;
    if (packet->getType().compare("RIP") == 0){
        auto rip = std::dynamic_pointer_cast<RipPacket>(packet);
        processRipPacket(rip, inputPort);
    }
    else if (packet->getType().compare("OSPF") == 0){
        auto ospf = std::dynamic_pointer_cast<OspfPacket>(packet);
        processOspfPacket(ospf, inputPort);
    }
    else if (packet->getType().compare("EBGP") == 0){

    }
    else if(packet->getType().compare("BGP") == 0 && isBorder){
        // auto ibgp = std::dynamic_pointer_cast<IBPGPacket>(packet);
        sendToOtherAS(packet);
    }
    else{

        forwardPacket(packet,inputPort);
    }
}



void Router::setibgpIps(std::vector<std::string> _ips){
    for (int i =0; i < _ips.size(); i++){
        ibgpIps.push_back(_ips[i]);
    }
}


std::string Router::findShortestIBGP(){
    int shortestCost = 1000;
    std::string shortestIp = "";
    std::string prot;
    if (routingProtocl == RIP){
        prot = "RIP";
    }
    else{
        prot = "OSPF";
    }

    for(int i =0 ; i < ibgpIps.size();i++){
        if (routingTable->getDestinationCost(ibgpIps[i],prot) < shortestCost){
            shortestCost = routingTable->getDestinationCost(ibgpIps[i],prot);
            shortestIp = ibgpIps[i];
        }
    }
    return shortestIp;
}


void Router::forwardPacket(std::shared_ptr<Packet> packet,int inputPort){

    std::string forwIp = packet->getDest();
    int nextPort = NO_WAY;
    std::string prot;
    if (routingProtocl == RIP){
        nextPort = routingTable->getOutputPort(forwIp,"RIP");
        prot = "RIP";
    }
    else if ( routingProtocl == OSPF){
        routingTable->getOutputPort(forwIp,"OSPF");
        prot = "OSPF";
    }

    if (nextPort == NO_WAY){
        packet->setType("BGP");
        std::string bgpIp = findShortestIBGP();
        packet->setFinalDest(packet->getDest());
        packet->setPacketDestination(bgpIp);
        int outport = routingTable->getOutputPort(bgpIp,prot);
        if (outport != NO_WAY){
            ports[outport]->addToOutBuffer(packet);
        }
    }
    else{
        ports[nextPort]->addToOutBuffer(packet);
    }


}

void Router::broadCast(std::shared_ptr<Packet> packet, RoutingProtocol rp){
    if (rp == OSPF){
        auto ospf = std::dynamic_pointer_cast<OspfPacket>(packet);
        for(int i =0; i < NUMBER_OF_PORTS; i++){
            ports[i]->addToOutBuffer(ospf.get()->copy());
        }
    }
    else{
        for(int i =0; i < NUMBER_OF_PORTS; i++){
            ports[i]->addToOutBuffer(packet);
        }
    }
}

void Router::StartOSPFProtocol(){
    Link* links = new Link();
    for (auto dest : neighbors.values()){
        if (dest.compare("") != 0)
            (*links)[dest] = 1;
    }
    std::shared_ptr<OspfPacket> packet = std::make_shared<OspfPacket>(ip, links);
    packet->addASNumber(AS);
    broadCast(packet);
}


void Router::StartEBGP(std::string routingProt){
    std::shared_ptr<EBPGPacket> packet = std::make_shared<EBPGPacket>("",ip);
    packet->addASNumber(AS);

    std::vector<std::string> prefixes = routingTable->createEbgpVector(routingProt);
    packet->addRoute(prefixes);
    broadCast(packet);
}



void Router::StartRIPProtocol(){
    std::shared_ptr<RipPacket> packet = std::make_shared<RipPacket>("",ip);
    packet->addASNumber(AS);
    QHash<std::string,int> distanceVector =routingTable->createRipDistanceVector();
    distanceVector[ip] = 0;
    for(int i =0; i< NUMBER_OF_PORTS; i++){
        if (!routingTable->hasDestIP(neighbors[i])){
            routingTable->insertRow(neighbors[i],"0.0.0.0",neighbors[i],i,1,"RIP");
        }
    }
    packet->addRoute(distanceVector);
    broadCast(packet);
}

void Router::processOspfPacket(std::shared_ptr<OspfPacket> packet, int inPort){
    if (lsdb.oldSequence(packet.get())){
        return;
    }

    lsdb.updateByOspfPacket(packet.get());
    routingTable->updateRoutingTableOSPF(lsdb, inPort);
    if (packet.get()->getTTL() > 0){
        packet.get()->decreaseTTL();
        broadCast(packet, OSPF);
    }
}

void Router::processRipPacket(std::shared_ptr<RipPacket> packet,int inPort){
    // std::cout << "got a rip packet"<< std::endl;
    QHash<std::string, int> newVector = packet.get()->getRoute();
    bool updated = false;
    QList keys = newVector.keys();
    for (int i =0 ; i <keys.size(); i++){
        newVector[keys[i]] += 1;
    }
    for (int i =0; i < keys.size(); i++){

        if (routingTable->hasDestIP(keys[i])){
            if (newVector[keys[i]] < routingTable->getDestinationCost(keys[i],"RIP")){
                routingTable->setDestinationCost(keys[i],"RIP",newVector[keys[i]]);
                routingTable->setOutputPort(keys[i],"RIP",inPort);
                updated = true;
            }
        }
        else{
            routingTable->insertRow(keys[i],"0.0.0.0",neighbors[inPort], inPort, newVector[keys[i]],"RIP");
            updated = true;
        }
    }
    if (updated){
        StartRIPProtocol();
        if (isBorder){
            StartEBGP("RIP");
        }
    }
}

void Router::printRoutingTable(){
    std::cout << *routingTable;
}


void Router::commandSlot(std::string command){
    if (command == ip){
        std::cout << *routingTable <<std::endl;
        if (isBorder){
            QList keys = BGPTable.keys();
            std::cout << "-------------------" <<std::endl;
            for (int i =0 ; i <keys.size(); i++){
                std::cout << "prefix: " << keys[i] << " from: " << BGPTable[keys[i]] << std::endl;
            }
            std::cout << "-------------------" <<std::endl;
        }
    }

}





void Router::setNeighbor(int port, std::string neighbor){
    neighbors[port] = neighbor;
}

std::string Router::getIp(){
    return ip;
}






