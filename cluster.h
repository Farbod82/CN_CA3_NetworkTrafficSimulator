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
private:
    int clusterNumber;
    std::vector<Router*> routers;
    std::vector<QThread*> threads;
    void connectTwoRouter(Router* r1, Router* r2);
signals:
};

#endif // CLUSTER_H
