#include "commandreader.h"
#include "clockgenerator.h"
#include "cluster.h"
#include <QThread>
#include <QtConcurrent>

Cluster::Cluster(int _clusterNumber,QObject *parent)
    : QObject{parent}
{
    clusterNumber = _clusterNumber;
}


void Cluster::createStarTopology(clockGenerator* clk, CommandReader* cmdr){
    std::vector<std::string> ipList = {"192.168.1.1","192.168.1.2","192.168.1.3","192.168.1.4"
                                  ,"192.168.1.5","192.168.1.6","192.168.1.7","192.168.1.8"};


    routers = {new Router(0,ipList[0],clusterNumber)};
    QThread* thread = new QThread();
    threads.push_back(thread);
    routers[0]->moveToThread(thread);
    QObject::connect(cmdr, &CommandReader::printRoutingTableRequested, routers[0], &Router::commandSlot);
    QObject::connect(clk, &clockGenerator::clockSignal, routers[0], &Router::processPacketsOnSignal);
    for(int i = 1; i  < 8 ; i++){
        Router* router = new Router(i,ipList[i],clusterNumber);
        QThread* thread = new QThread();
        routers.push_back(router);
        threads.push_back(thread);
        router->moveToThread(thread);
        QObject::connect(routers[i-1]->ports[0], &Buffer::sendPacketSignal, routers[i%7]->ports[1], &Buffer::recievePacket);
        QObject::connect(routers[i%7]->ports[1], &Buffer::sendPacketSignal, routers[i-1]->ports[0], &Buffer::recievePacket);
        QObject::connect(clk, &clockGenerator::clockSignal, routers[i], &Router::processPacketsOnSignal);
        QObject::connect(cmdr, &CommandReader::printRoutingTableRequested, routers[i], &Router::commandSlot);
    }
    QThread* threadMid = threads[7];
    Router* router = routers[7];
    router->moveToThread(threadMid);
    QObject::connect(router->ports[0], &Buffer::sendPacketSignal, routers[1]->ports[2], &Buffer::recievePacket);
    QObject::connect(routers[1]->ports[2], &Buffer::sendPacketSignal, router->ports[0], &Buffer::recievePacket);

    QObject::connect(router->ports[1], &Buffer::sendPacketSignal, routers[2]->ports[2], &Buffer::recievePacket);
    QObject::connect(routers[2]->ports[2], &Buffer::sendPacketSignal, router->ports[1], &Buffer::recievePacket);

    QObject::connect(router->ports[2], &Buffer::sendPacketSignal, routers[4]->ports[2], &Buffer::recievePacket);
    QObject::connect(routers[4]->ports[2], &Buffer::sendPacketSignal, router->ports[2], &Buffer::recievePacket);

    QObject::connect(router->ports[3], &Buffer::sendPacketSignal, routers[6]->ports[2], &Buffer::recievePacket);
    QObject::connect(routers[6]->ports[2], &Buffer::sendPacketSignal, router->ports[3], &Buffer::recievePacket);

    for(int i =0 ; i < 8; i++){
        threads[i]->start();
    }
}

void Cluster::startRouting(){
    QtConcurrent::run(&Router::StartRIPProtocol, routers[0]);
    // QThread::sleep(1);
    // for (int i =0; i < 8; i++){
    //     routers[i]->printRoutingTable();
    // }
}

