#ifndef CLUSTER_H
#define CLUSTER_H

#include "commandReader.h"
#include "clockGenerator.h"
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
private:
    int clusterNumber;
    std::vector<Router*> routers;
    std::vector<QThread*> threads;
signals:
};

#endif // CLUSTER_H
