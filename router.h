#ifndef ROUTER_H
#define ROUTER_H

#include "node.h"
#include "routingtable.h"
#include "rippacket.h"
#include "buffer.h"

#include <string>
#include <vector>
#include <QObject>
#include <QHash>


#define NUMBER_OF_PORTS 5

typedef enum RoutingProtocol{
    RIP,
    OSPF,
} RoutingProtocol;


class Router : public Node
{
    Q_OBJECT
public:
    explicit Router(int _id, std::string _ip, int _AS, QObject *parent = nullptr);
    void createPacket(int outPort);
    void send(std::shared_ptr<std::string> data);

    std::vector<Buffer*> ports;

    void processPackets(std::shared_ptr<Packet> packet, int inputPort);
    void processOspfPacket(std::shared_ptr<OspfPacket> packet, int inPort);
    void broadCast(std::shared_ptr<Packet> packet, RoutingProtocol rp = RIP);
    void StartRIPProtocol();
    void StartOSPFProtocol();
    void processRipPacket(std::shared_ptr<RipPacket> packet, int inPort);
    void printRoutingTable();
    void setNeighbor(int port, std::string neighbor);
    std::string getIp();
    void setAsBorder();
    void processBGP(std::shared_ptr<Packet> packet, int inputPort);
    void StartEBGP(std::string routingProt);
    QHash<std::string,std::string> BGPTable;
    bool DoesBGPTableContain(std::string prefix);
    void forwardPacket(std::shared_ptr<Packet> packet, int inputPort);
public slots:
    void commandSlot(std::string command);
    void processPacketsOnSignal();
    // void ospfBroadCastLinkCostChange();
    // void changeRoutingProtocol(RoutingProtocol _rp);
private:
    bool isBorder;
    int id;
    int AS;
    std::string ip;
    QHash<std::string, int> distanceVector;
    QHash<std::string, int> shoretestPathPorts;
    LSDB lsdb;
    RoutingTable* routingTable;
    RoutingProtocol routingProtocl = RIP;
    QHash<int, std::string> neighbors;
};

#endif // ROUTER_H
