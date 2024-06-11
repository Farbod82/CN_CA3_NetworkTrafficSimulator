#include "packet.h"
#include "packetsaver.h"
#include <QThread>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;


PacketSaver::PacketSaver(QObject *parent) : QObject(parent)
{
}

std::vector<std::shared_ptr<Packet>> PacketSaver::getSavedPackets(){
    return savedPackets;
}

void PacketSaver::savePackets(std::shared_ptr<Packet> _packet){
    savedPackets.push_back(_packet);
}

