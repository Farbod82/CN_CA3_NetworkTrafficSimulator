#include "router.h"
#include "node.h"

#include <iostream>
#include <bits/shared_ptr.h>
#include <string>

Router::Router(QObject *parent)
    : Node{parent}
{

}

void Router::send(std::shared_ptr<std::string> data){
    std::cout << "sending back" << std::endl;
    emit sendPacket(data);
}


void Router::recievePacket(std::shared_ptr<std::string> data){
    std::cout << "recieved: " << data->data() << std::endl;
    send(data);
}

void Router::createPacket(){
    std::shared_ptr<std::string> sharedData = std::make_shared<std::string>("TA Gave");
    emit sendPacket(sharedData);
}


