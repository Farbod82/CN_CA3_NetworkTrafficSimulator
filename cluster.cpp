#include "commandreader.h"
#include "clockgenerator.h"
#include "cluster.h"
#include "router.h"
#include <QThread>
#include <QtConcurrent>

Cluster::Cluster(int _clusterNumber,QObject *parent)
    : QObject{parent}
{
    clusterNumber = _clusterNumber;
}

// void Cluster::connectChangeRoutingProtocolSignal(){
//     for (auto rt : routers){
//         QObject::connect(this, &Cluster::changeRoutingProtocol, rt, &Router::changeRoutingProtocol);
//     }
// }


// void Cluster::changeRoutingProtocol(RoutingProtocol _rp){
//     emit changeRoutingProtocol(_rp);
// }


void Cluster::connectTwoRouters(Router* r1, int p1, Router* r2, int p2){
        QObject::connect(r1->ports[p1], &Buffer::sendPacketSignal, r2->ports[p2], &Buffer::recievePacket);
        QObject::connect(r2->ports[p2], &Buffer::sendPacketSignal, r1->ports[p1], &Buffer::recievePacket);
        r1->setNeighbor(p1, r2->getIp());
        r2->setNeighbor(p2, r1->getIp());
}

void Cluster::createStarTopology(clockGenerator* clk, CommandReader* cmdr){
    std::vector<std::string> ipList = {"192.168.1.1","192.168.1.2","192.168.1.3","192.168.1.4"
                                  ,"192.168.1.5","192.168.1.6","192.168.1.7","192.168.1.8"};


    routers.push_back(new Router(0, ipList[0], clusterNumber));
    QThread* thread = new QThread();
    threads.push_back(thread);
    routers[0]->moveToThread(thread);
    QObject::connect(cmdr, &CommandReader::printRoutingTableRequested, routers[0], &Router::commandSlot);
    QObject::connect(clk, &clockGenerator::clockSignal, routers[0], &Router::processPacketsOnSignal);
    for(int i = 1; i  < 8 ; i++){
        Router* router = new Router(i, ipList[i], clusterNumber);
        QThread* thread = new QThread();
        routers.push_back(router);
        threads.push_back(thread);
        router->moveToThread(thread);
        connectTwoRouters(routers[i-1], 0, routers[i%7], 1);
        QObject::connect(clk, &clockGenerator::clockSignal, routers[i], &Router::processPacketsOnSignal);
        QObject::connect(cmdr, &CommandReader::printRoutingTableRequested, routers[i], &Router::commandSlot);
    }

    QThread* threadMid = threads[7];
    Router* router = routers[7];
    router->moveToThread(threadMid);

    connectTwoRouters(router, 0, routers[1], 2);
    connectTwoRouters(router, 1, routers[2], 2);
    connectTwoRouters(router, 2, routers[4], 2);
    connectTwoRouters(router, 3, routers[6], 2);


    // for(int i =0 ; i < 8; i++){
    //     threads[i]->start();
    // }
}


void Cluster::startThreads(){
    for(int i=0; i < threads.size(); i++){
        threads[i]->start();
    }
}

void Cluster::createMeshTopology(clockGenerator* clk, CommandReader* cmdr){
    std::vector<std::string> ipList = {"192.168.1.9","192.168.1.10","192.168.1.11","192.168.1.12"
                                       ,"192.168.1.13","192.168.1.14","192.168.1.15","192.168.1.16","192.168.1.17","192.168.1.18","192.168.1.19", "192.168.1.20","192.168.1.21","192.168.1.22","192.168.1.23","192.168.1.24"};

    for (int i =0; i < 4; i++){
        Router* router1 = new Router(i*4,ipList[i*4],clusterNumber);
        QThread* thread1 = new QThread();
        routers.push_back(router1);
        threads.push_back(thread1);
        router1->moveToThread(thread1);
        if (i != 0){
            QObject::connect(router1->ports[3], &Buffer::sendPacketSignal, routers[(i-1)*4]->ports[2], &Buffer::recievePacket);
            QObject::connect(routers[(i-1)*4]->ports[2], &Buffer::sendPacketSignal, router1->ports[3], &Buffer::recievePacket);

        }
        QObject::connect(clk, &clockGenerator::clockSignal, router1, &Router::processPacketsOnSignal);
        QObject::connect(cmdr, &CommandReader::printRoutingTableRequested, router1, &Router::commandSlot);
        for (int j = 1 ; j < 4; j++){
            Router* router = new Router((i*4)+j,ipList[(i*4)+j],clusterNumber);
            QThread* thread = new QThread();
            routers.push_back(router);
            threads.push_back(thread);
            router->moveToThread(thread);
            if (i != 0){
                QObject::connect(router->ports[3], &Buffer::sendPacketSignal, routers[((i-1)*4)+j]->ports[2], &Buffer::recievePacket);
                QObject::connect(routers[((i-1)*4)+j]->ports[2], &Buffer::sendPacketSignal, router->ports[3], &Buffer::recievePacket);
            }
            QObject::connect(router->ports[1], &Buffer::sendPacketSignal, routers[((i)*4)+j-1]->ports[0], &Buffer::recievePacket);
            QObject::connect(routers[((i)*4)+j-1]->ports[0], &Buffer::sendPacketSignal, router->ports[1], &Buffer::recievePacket);
            QObject::connect(clk, &clockGenerator::clockSignal, router, &Router::processPacketsOnSignal);
            QObject::connect(cmdr, &CommandReader::printRoutingTableRequested, router, &Router::commandSlot);
        }
    }
}


void Cluster::addStarToMesh(Cluster* starCluster){

    QObject::connect(routers[7]->ports[4], &Buffer::sendPacketSignal, starCluster->routers[6]->ports[4], &Buffer::recievePacket);
    QObject::connect(starCluster->routers[0]->ports[4], &Buffer::sendPacketSignal, routers[11]->ports[4], &Buffer::recievePacket);

    QObject::connect(routers[11]->ports[4], &Buffer::sendPacketSignal, starCluster->routers[0]->ports[4], &Buffer::recievePacket);
    QObject::connect(starCluster->routers[0]->ports[4], &Buffer::sendPacketSignal, routers[11]->ports[4], &Buffer::recievePacket);

    QObject::connect(routers[15]->ports[4], &Buffer::sendPacketSignal, starCluster->routers[1]->ports[4], &Buffer::recievePacket);
    QObject::connect(starCluster->routers[1]->ports[4], &Buffer::sendPacketSignal, routers[15]->ports[4], &Buffer::recievePacket);
}

void Cluster::startRouting(){
    QtConcurrent::run(&Router::StartRIPProtocol, routers[0]);
    // QtConcurrent::run(&Router::StartOSPFProtocol, routers[0]);
    // QThread::sleep(1);
    // for (int i =0; i < 8; i++){
    //     routers[i]->printRoutingTable();
    // }
}




