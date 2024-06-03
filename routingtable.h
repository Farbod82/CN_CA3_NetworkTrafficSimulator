#ifndef ROUTINGTABLE_H
#define ROUTINGTABLE_H

#include <QObject>
#include "lsbd.h"

const int NO_WAY = -1;

class RoutingTable: public QObject
{
    Q_OBJECT
public:
    explicit RoutingTable(std::string);
    virtual ~RoutingTable();
    bool insertRow(std::string, std::string, std::string, int, int, std::string);
    bool editRowBaseOneDestinationAndProtocol(std::string dest, std::string subMask,
                                              std::string gate, int port, int metr, std::string prot);
    int getOutputPort(std::string, std::string);
    int getDestinationCost(std::string , std::string);
    bool removeRow(std::string _destination, std::string _protocol);
    void updateRoutingTableOSPF(LSDB* lsdb);

private:
    std::string routerIp;
    std::vector<std::string> destAddr;
    std::vector<std::string> subnetMask;
    std::vector<std::string> gateWayMask;
    std::vector<int> interfacePort;
    std::vector<int> metric;
    std::vector<std::string> protocol;


};

#endif // ROUTINGTABLE_H
