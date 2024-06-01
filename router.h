#ifndef ROUTER_H
#define ROUTER_H

#include "node.h"
#include <string>
#include <vector>

#include <QObject>
#include <QHash>
#include "buffer.h"
#include "rippacket.h"


#define NUMBER_OF_PORTS 5




class Router : public Node
{
    Q_OBJECT
public:
    explicit Router(int _id,std::string ip,int AS, QObject *parent = nullptr);
    void createPacket(int outPort);
    void send(std::shared_ptr<std::string> data);

    std::vector<Buffer*> ports;

    void processPackets(std::shared_ptr<Packet> packet, int inputPort);
    void broadCast(std::shared_ptr<Packet> packet);
    void StartRIPProtocol();
    void processRipPacket(std::shared_ptr<RipPacket> packet, int inPort);
    void printRoutingTable();
public slots:
    void commandSlot(std::string command);
    void processPacketsOnSignal();
private:
    int id;
    int AS;
    std::string ip;
    QHash<std::string, int> distanceVector;
    QHash<std::string, int> shoretestPathPorts;
};

#endif // ROUTER_H
