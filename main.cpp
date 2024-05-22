#include <QCoreApplication>

#include "router.h"
#include <qthread.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread thread1;
    QThread thread2;

    // Create the Router objects
    Router router1;
    Router router2;

    // Move the Router objects to their respective threads
    router1.moveToThread(&thread1);
    router2.moveToThread(&thread2);

    // Connect the signals and slots
    QObject::connect(&router1, &Router::sendPacket, &router2, &Router::recievePacket);
    QObject::connect(&router2, &Router::sendPacket, &router1, &Router::recievePacket);


    // Start the threads
    thread1.start();
    thread2.start();

    // Example usage: Create a packet in router1 and send it to router2
    router1.createPacket();

    // Wait for the threads to finish
    // thread1.quit();
    // thread1.wait();
    // thread2.quit();
    // thread2.wait();

    return a.exec();
}
