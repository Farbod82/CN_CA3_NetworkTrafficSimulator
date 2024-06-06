#ifndef CLUSTER_H
#define CLUSTER_H

#include "commandreader.h"
#include "clockgenerator.h"
#include "router.h"
#include <QObject>

class Cluster : public QObject
{
    Q_OBJECT
public:
    explicit Cluster(int _clusterNumber,QObject *parent = nullptr);
    void startRouting();
    void createStarTopology(clockGenerator *clk, CommandReader *cmdr);
    void startThreads();
    void createMeshTopology(clockGenerator *clk, CommandReader *cmdr);
    void addStarToMesh(Cluster *starCluster);
private:
    int clusterNumber;
    RoutingProtocol routingProtocol;
    std::vector<Router*> routers;
    std::vector<QThread*> threads;
    void connectTwoRouters(Router* r1, int p1, Router* r2, int p2);
    void connectHost(Router* rt, int rp, Host* ht, int hp);
    // void connectChangeRoutingProtocolSignal();

// signals:
//     void changeRoutingProtocol(RoutingProtocol _rp);
};

#endif // CLUSTER_H
