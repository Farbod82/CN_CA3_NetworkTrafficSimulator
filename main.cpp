#include <QCoreApplication>
#include "clockgenerator.h"

#include "commandreader.h"
#include "cluster.h"
#include "router.h"
#include "packetsaver.h"
#include <qthread.h>
#include <QtConcurrent>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread thread1;
    QThread thread2;
    QThread thread3;
    QThread thread4;
    QThread thread5;
    PacketSaver packetSaver;
    clockGenerator clk;
    CommandReader cmndr;
    Cluster cluster(1);
    Cluster cluster2(2);
    cmndr.setPacketSaver(&packetSaver);
    cluster.moveToThread(&thread2);
    cluster2.moveToThread(&thread4);
    clk.moveToThread(&thread1);
    cmndr.moveToThread(&thread3);
    packetSaver.moveToThread(&thread5);


    cluster2.createStarTopology(&clk,&cmndr, &packetSaver);
    cluster.createMeshTopology(&clk,&cmndr, &packetSaver);
    cluster.addStarToMesh(&cluster2);
    cluster.startThreads();
    cluster2.startThreads();

    thread1.start();
    thread2.start();
    thread3.start();
    thread4.start();
    thread5.start();

    QtConcurrent::run(&clockGenerator::startGeneration, &clk);
    QtConcurrent::run(&Cluster::startRouting, &cluster);
    QtConcurrent::run(&Cluster::startRouting, &cluster2);
    QtConcurrent::run(&CommandReader::readCommands, &cmndr);
    return a.exec();
}
