#include "host.h"

#include <iostream>
#include <random>
#include <cmath>
#include <time.h>

Host::Host(std::string _ip, double pareto_alpha, double pareto_xm, int _as, QObject *parent)
    : QObject{parent}{

    ip = _ip;
    alpha = pareto_alpha;
    xm = pareto_xm;
    port = new Buffer(0);
    srand(time(NULL));
    generator = new std::default_random_engine(time(NULL));
    AS = _as;
}

void Host::setPartners(const std::vector<std::string>& _partners){
    for (std::string p: _partners){
        partners.push_back(p);
    }

}

bool Host::regularType(Packet* packet){
    if (packet->getType().compare("RIP") == 0){
        return false;
    }
    else if (packet->getType().compare("OSPF") == 0){
        return false;
    }
    else if (packet->getType().compare("EBGP") == 0){
        return false;
    }
    else if(packet->getType().compare("IBGP") == 0){
        return false;
    }
    else{
        return true;
    }

}

void Host::createAndSendPacket(){
    int random = rand()%partners.size();
    std::string choosed_partner = partners[random];
    std::shared_ptr<Packet> packet = std::make_shared<Packet>(ip, choosed_partner, "packet");
    packet.get()->setBody("Besme Allah Alrahman Alrahim");
    packet.get()->addASNumber(AS);
    std::cout << "Packet sent"<<std::endl;
    port->addToOutBuffer(packet);
}

double Host::pareto_distribution() {
    std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);
    double u = uniform_dist(*generator);
    return xm / std::pow(1 - u, 1.0 / alpha);
}


void Host::parteoSendPacket(){
    double pareto = pareto_distribution();
    double random = rand();
    if (random < pareto){
        createAndSendPacket();
    }
}

void Host::handlePackets(){
    std::shared_ptr<Packet> packet = port->getFirstPacket();
    if (packet.get() != nullptr &&
        regularType(packet.get())){
        std::cout << packet.get()->getSource() << std::endl;
    }
    port->sendPacket();
}

std::string Host::getIp(){
    return ip;
}
