#ifndef CLUSTER_H
#define CLUSTER_H

#include "commandreader.h"
#include "clockgenerator.h"
#include "router.h"
#include "host.h"
#include <QObject>

class Cluster : public QObject
{
    Q_OBJECT
public:
    explicit Cluster(int _clusterNumber,QObject *parent = nullptr);
    void startRouting();
    void startThreads();
    void addStarToMesh(Cluster *starCluster);
    void createStarTopology(clockGenerator *clk, CommandReader *cmdr, PacketSaver *packetSaver);
    void createMeshTopology(clockGenerator *clk, CommandReader *cmdr, PacketSaver *packetSaver);
private:
    int clusterNumber;
    RoutingProtocol routingProtocol;
    std::vector<Router*> routers;
    std::vector<QThread*> threads;
    void connectTwoRouters(Router* r1, int p1, Router* r2, int p2);
    void connectHost(Router* rt, int rp, Host* ht);
    std::vector<std::string> host_ip1 = {"192.168.101", "192.168.102"};
    std::vector<std::string> host_ip2 = {"192.168.201", "192.168.202"};
    // void connectChangeRoutingProtocolSignal();

// signals:
//     void changeRoutingProtocol(RoutingProtocol _rp);
};

#endif // CLUSTER_H
