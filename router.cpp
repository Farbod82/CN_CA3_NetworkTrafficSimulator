#include "router.h"
#include "node.h"
#include "rippacket.h"
#include "ebgppacket.h"

#include <iostream>
#include <bits/shared_ptr.h>
#include <string>

#include "routingtable.h"



Router::Router(int _id, std::string _ip, int _AS, QObject *parent)
    : Node(_id,parent)
{
    isBorder = false;
    AS = _AS;
    ip = _ip;
    id = _id;
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

void Router::createPacket(int outPort){
    std::shared_ptr<Packet> packet = std::make_shared<Packet>("123123","1233213","packet");
    packet->setBody("I hate CN");
    std::cout << "hoooooooooooooooooooooooy"<<std::endl;
    // ports[outPort]->addToOutBuffer(packet);
    broadCast(packet);
}

void Router::processPacketsOnSignal(){

    for (int i =0 ; i < NUMBER_OF_PORTS; i++){
        std::shared_ptr<Packet> packet = ports[i]->getFirstPacket();
        if (packet.get() != nullptr){
            if (packet->getInitialASNumber() == AS){
                processPackets(packet,i);
            }
            if (packet->getInitialASNumber() != AS && isBorder){
                processBGP(packet,i);
            }
        }
        ports[i]->sendPacket();
    }

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
}

void Router::processPackets(std::shared_ptr<Packet> packet,int inputPort){
    // std::cout << "router " << id << " got packet " << packet.get()->getBody() << " on port " << inputPort << std::endl;
    if (packet->getType().compare("RIP") == 0){
        auto rip = std::dynamic_pointer_cast<RipPacket>(packet);
        processRipPacket(rip, inputPort);
    }
    else if (packet->getType().compare("OSPF") == 0){
        auto ospf = std::dynamic_pointer_cast<OspfPacket>(packet);
        processOspfPacket(ospf);
    }
    else if (packet->getType().compare("EBGP") == 0){

    }
    else if(packet->getType().compare("IBGP") == 0){
         //todo
    }
    else{
        forwardPacket(packet,inputPort);
    }
}

void Router::forwardPacket(std::shared_ptr<Packet> packet,int inputPort){
    std::string forwIp = packet->getDest();
    int nextPort = NO_WAY;
    if (routingProtocl == RIP){
        nextPort = routingTable->getOutputPort(forwIp,"RIP");
    }
    else if ( routingProtocl == OSPF){
        routingTable->getOutputPort(forwIp,"OSPF");
    }

    if (nextPort == NO_WAY){
           //do ibgp
    }
    else{
        ports[nextPort]->addToOutBuffer(packet);
    }


}

void Router::broadCast(std::shared_ptr<Packet> packet){
    for(int i =0; i < NUMBER_OF_PORTS; i++){
        ports[i]->addToOutBuffer(packet);
    }
}

void Router::StartOSPFProtocol(){
    Link links;
    for (auto dest : neighbors.values()){
        links[dest] = 1;
    }
    std::shared_ptr<OspfPacket> packet = std::make_shared<OspfPacket>(ip, links);
    packet->addASNumber(AS);
    broadCast(packet);
}


void Router::StartEBGP(std::string routingProt){
    std::shared_ptr<EBPGPacket> packet = std::make_shared<EBPGPacket>("",ip);
    packet->addASNumber(AS);
    std::vector<std::string> prefixes = routingTable->createEbgpVector(routingProt);
    // for (auto it = BGPTable.begin(); it != BGPTable.end(); ++it) {
    //     prefixes.push_back(it.key());
    // }
    packet->addRoute(prefixes);
    broadCast(packet);
}



void Router::StartRIPProtocol(){
    std::shared_ptr<RipPacket> packet = std::make_shared<RipPacket>("",ip);
    packet->addASNumber(AS);
    QHash<std::string,int> distanceVector =routingTable->createRipDistanceVector();
    distanceVector[ip] = 0;
    packet->addRoute(distanceVector);
    broadCast(packet);
}

void Router::processOspfPacket(std::shared_ptr<OspfPacket> packet){
    lsdb.updateByOspfPacket(packet.get());
    routingTable->updateRoutingTableOSPF(lsdb);
    if (packet.get()->getTTL() > 1){
        packet.get()->decreaseTTL();
        broadCast(packet);
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
            routingTable->insertRow(keys[i],"f",neighbors[inPort], inPort, newVector[keys[i]],"RIP");
            updated = true;
        }
    }
    if (updated){
        StartRIPProtocol();
        if (isBorder){
            StartEBGP("RIP");
        }
        // std::cout << "-----------------------------------------" <<std::endl;
        // printRoutingTable();
        // std::cout << "-----------------------------------------" <<std::endl;
    }
}

void Router::printRoutingTable(){
    QList keys = distanceVector.keys();
    std::cout << "router: " << id << std::endl;
    for(int i =0; i < keys.size(); i++){
        std::cout << "ip: " <<keys[i] << " destinationPort: " << shoretestPathPorts[keys[i]] << " distance: " << distanceVector[keys[i]] << std::endl;
    }
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



// void Router::changeRoutingProtocol(RoutingProtocol _rp){
//     routingProtocl = _rp;
// }

void Router::setNeighbor(int port, std::string neighbor){
    neighbors[port] = neighbor;
}

std::string Router::getIp(){
    return ip;
}






