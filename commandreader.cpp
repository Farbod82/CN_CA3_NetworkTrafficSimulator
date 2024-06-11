#include "commandreader.h"
#include "ebgppacket.h"
#include "packetsaver.h"
#include <QThread>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

CommandReader::CommandReader(QObject *parent) : QObject(parent)
{
}


void CommandReader::setPacketSaver(PacketSaver* _packetSaver){
    packetSaver = _packetSaver;
}

void CommandReader::readCommands()
{
    while (true)
    {
        std::string command;
        std::cout << "Enter a command: ";
        std::getline(std::cin, command);

        istringstream iss(command);
        string s;
        vector<std::string> commandSplits;
        while ( getline( iss, s, ' ' ) ) {
            commandSplits.push_back(s);
        }
        if (commandSplits.empty()){
            continue;
        }
        std::cout <<commandSplits[0] <<std::endl;
        if(commandSplits[0].compare("print") == 0)
            emit printRoutingTableRequested(commandSplits[1]);
        if(commandSplits[0].compare("packets") == 0){
            printAllPackets();
        }
        if(commandSplits[0].compare("path") == 0){
            printPacket(commandSplits[1]);
        }
        QThread::sleep(1);
    }
}

bool CommandReader::checkPrintRoutingTable(const std::string &command)
{
    return command.compare("print") == 0;
}


void CommandReader::printAllPackets(){
    std::vector<std::shared_ptr<Packet>> packets = packetSaver->getSavedPackets();
    for (int  i= 0; i < packets.size(); i++){
        std::cout << "packet: " << i << std::endl;
        std::cout <<"source: " <<packets[i]->getSource() << std::endl;
        std::cout <<"dest: " <<packets[i]->getDest() << std::endl;
        // std::cout << "buffer cycles: " << packets[i]->getBufferCycles() << std::endl;
        // std::cout << "delivery cycles: " << packets[i]->getDeliveryCycles() << std::endl;
    }
}
void CommandReader::printPacket(std::string packetNum){
    int i = stoi(packetNum);
    std::vector<std::shared_ptr<Packet>> packets = packetSaver->getSavedPackets();
    std::cout <<"type: " << packets[i]->getType() <<std::endl;
    std::cout <<"source: " <<packets[i]->getSource() << std::endl;
    std::cout <<"dest: " <<packets[i]->getDest() << std::endl;
    std::cout << "buffer cycles: " << packets[i]->getBufferCycles() << std::endl;
    std::cout << "delivery cycles: " << packets[i]->getDeliveryCycles() << std::endl;
    std::vector<std::string> ips = packets[i]->getPath();
    std::cout << ips.size()<<std::endl;
    for (int j = 0; j < ips.size(); j++){
        std::cout << "dest: " << j << ": " << ips[j] << std::endl;
    }
}




