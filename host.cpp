#include "host.h"

#include <iostream>
#include <random>
#include <cmath>
#include <time.h>

Host::Host(std::string _ip, double pareto_alpha, double pareto_xm, QObject *parent)
    : QObject{parent}{

    ip = _ip;
    alpha = pareto_alpha;
    xm = pareto_xm;
    port = new Buffer(0);
    srand(time(NULL));
    generator = new std::default_random_engine(time(NULL));
}

void Host::setPartners(std::vector<std::string>* _partners){
    partners  = _partners;
}

void Host::createAndSendPacket(){
    int random = rand();
    std::string choosed_partner = (*partners)[random%partners->size()];
    std::shared_ptr<Packet> packet = std::make_shared<Packet>(ip, choosed_partner, "packet");
    packet->setBody("Besme Allah Alrahman Alrahim");
    std::cout << "Packet sent"<<std::endl;
    // ports[outPort]->addToOutBuffer(packet);
    port->addToOutBuffer(packet);
}

double Host::pareto_distribution() {
    // Generate a uniform random variable between 0 and 1
    std::uniform_real_distribution<double> uniform_dist(0.0, 1.0);
    double u = uniform_dist(*generator);

    // Apply the inverse transform method for Pareto distribution
    return xm / std::pow(1 - u, 1.0 / alpha);
}


void Host::parteoSendPacket(){
    double pareto = pareto_distribution();
    double random = rand();
    if (random < pareto){
        createAndSendPacket();
    }
}

void Host::receivePackets(){
    std::shared_ptr<Packet> packet = port->getFirstPacket();
    if (packet.get() != nullptr){
        std::cout << packet.get()->getSource() << std::endl;
    }
}

std::string Host::getIp(){
    return ip;
}
