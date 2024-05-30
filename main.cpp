#include <QCoreApplication>
#include "clockGenerator.h"

#include "router.h"
#include <qthread.h>
#include <QtConcurrent>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread thread1;
    QThread thread2;
    QThread thread3;
    QThread thread4;
    // Create the Router objects
    Router router1(1);
    Router router2(2);
    Router router3(3);
    clockGenerator clk;

    // Move the Router objects to their respective threads
    router1.moveToThread(&thread1);
    router2.moveToThread(&thread2);
    router3.moveToThread(&thread4);
    clk.moveToThread(&thread3);


    // Connect the signals and slots
    QObject::connect(router1.ports[0], &Buffer::sendPacketSignal, router2.ports[4], &Buffer::recievePacket);
    QObject::connect(router2.ports[4], &Buffer::sendPacketSignal, router1.ports[0], &Buffer::recievePacket);
    QObject::connect(router1.ports[1], &Buffer::sendPacketSignal, router2.ports[3], &Buffer::recievePacket);
    QObject::connect(router2.ports[3], &Buffer::sendPacketSignal, router2.ports[1], &Buffer::recievePacket);
    QObject::connect(&clk, &clockGenerator::clockSignal, &router1, &Router::processPacketsOnSignal);
    QObject::connect(&clk, &clockGenerator::clockSignal, &router2, &Router::processPacketsOnSignal);
    QObject::connect(&clk, &clockGenerator::clockSignal, &router3, &Router::processPacketsOnSignal);


    thread1.start();
    thread2.start();
    thread3.start();
    thread4.start();
    QtConcurrent::run(&clockGenerator::startGeneration, &clk);
    QtConcurrent::run(&Router::createPacket, &router1,0);

    return a.exec();
}
