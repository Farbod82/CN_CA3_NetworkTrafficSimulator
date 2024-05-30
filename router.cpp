#include "router.h"
#include "node.h"

#include <iostream>
#include <bits/shared_ptr.h>
#include <string>




Router::Router(int _id,QObject *parent)
    : Node{_id,parent}
{
    id = _id;
    for(int i =0; i < NUMBER_OF_PORTS; i++){
        Buffer* buffer = new Buffer(i);
        ports.push_back(buffer);
    }
}


void Router::processPacketsOnSignal(){
    std::cout << "hi?" << this->id <<std::endl;
    for (int i =0 ; i < NUMBER_OF_PORTS; i++){
        std::shared_ptr<Packet> packet = ports[i]->getFirstPacket();
        if (packet.get() != nullptr){
            processPackets(packet,i);
        }
        ports[i]->sendPacket();
    }
}

void Router::processPackets(std::shared_ptr<Packet> packet,int inputPort){
    std::cout << "router " << id << " got packet " << packet.get()->getBody() << " on port " << inputPort << std::endl;
    ports[inputPort]->addToOutBuffer(packet);
}



void Router::createPacket(int outPort){
    std::shared_ptr<Packet> packet = std::make_shared<Packet>("123123","1233213","packet");
    packet->setBody("I hate CN");
    std::cout << "hoooooooooooooooooooooooy"<<std::endl;
    ports[outPort]->addToOutBuffer(packet);

}








