#ifndef ROUTER_H
#define ROUTER_H

#include "node.h"
#include <string>
#include <vector>

#include <QObject>
#include <QHash>
#include "buffer.h"



#define NUMBER_OF_PORTS 5




class Router : public Node
{
    Q_OBJECT
public:
    explicit Router(int _id, QObject *parent = nullptr);
    void createPacket(int outPort);
    void send(std::shared_ptr<std::string> data);

    std::vector<Buffer*> ports;

    void processPackets(std::shared_ptr<Packet> packet, int inputPort);
    void broadCast(std::shared_ptr<Packet> packet);
public slots:
    void processPacketsOnSignal();
private:
    int id;
    std::string ip;
    QHash<std::string, QHash<std::string, int>> distanceVector;
};

#endif // ROUTER_H
