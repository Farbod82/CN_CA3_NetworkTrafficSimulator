#include "router.h"
#include "node.h"
#include "rippacket.h"

#include <iostream>
#include <bits/shared_ptr.h>
#include <string>

#include "routingtable.h"



Router::Router(int _id, std::string _ip, int _AS, QObject *parent)
    : Node{_id,parent}
{
    AS = _AS;
    ip = _ip;
    id = _id;
    distanceVector[ip] = 0;
    shoretestPathPorts[ip] = 0;
    for(int i =0; i < NUMBER_OF_PORTS; i++){
        Buffer* buffer = new Buffer(i);
        ports.push_back(buffer);
        neighbors[i] = "";
    }
    lsdb = new LSDB();
    routingTable = new RoutingTable(ip);
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
            processPackets(packet,i);
        }
        ports[i]->sendPacket();
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
    broadCast(packet);
}

void Router::StartRIPProtocol(){
    std::shared_ptr<RipPacket> packet = std::make_shared<RipPacket>("",ip);
    packet->addRoute(distanceVector);
    broadCast(packet);
}

void Router::processOspfPacket(std::shared_ptr<OspfPacket> packet){
    lsdb->updateByOspfPacket(packet.get());
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
        if (distanceVector.contains(keys[i])){
            if (newVector[keys[i]] < distanceVector[keys[i]]){
                distanceVector[keys[i]] = newVector[keys[i]];
                shoretestPathPorts[keys[i]] = inPort;
                updated = true;
            }
        }
        else{
            distanceVector[keys[i]] = newVector[keys[i]];
            shoretestPathPorts[keys[i]] = inPort;
            updated = true;
        }
    }
    if (updated){
        StartRIPProtocol();
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
    if (stoi(command) == id)
        printRoutingTable();
}


void Router::changeRoutingProtocol(RoutingProtocol _rp){
    routingProtocl = _rp;
}

void Router::setNeighbor(int port, std::string neighbor){
    neighbors[port] = neighbor;
}

std::string Router::getIp(){
    return ip;
}



