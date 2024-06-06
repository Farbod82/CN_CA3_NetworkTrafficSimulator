#include <QCoreApplication>
#include "clockGenerator.h"

#include "CommandReader.h"
#include "cluster.h"
#include "router.h"
#include <qthread.h>
#include <QtConcurrent>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread thread1;
    QThread thread2;
    QThread thread3;
    clockGenerator clk;
    CommandReader cmndr;
    Cluster cluster(1);
    cluster.moveToThread(&thread2);
    clk.moveToThread(&thread1);
    cmndr.moveToThread(&thread3);

    // cluster.createStarTopology(&clk,&cmndr);
    cluster.createMeshTopology(&clk,&cmndr);
    cluster.startThreads();

    thread1.start();
    thread2.start();
    thread3.start();

    QtConcurrent::run(&clockGenerator::startGeneration, &clk);
    QtConcurrent::run(&Cluster::startRouting, &cluster);
    QtConcurrent::run(&CommandReader::readCommands, &cmndr);
    return a.exec();
}
