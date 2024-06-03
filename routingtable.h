#ifndef ROUTINGTABLE_H
#define ROUTINGTABLE_H

#include <QObject>

class RoutingTable: public QObject
{
    Q_OBJECT
public:
    RoutingTable();
    virtual ~RoutingTable();
    bool insertRow(std::string, std::string, std::string, int, int, std::string);
    bool editRow(std::string, std::string, std::string, int, int, std::string);
    int getOutputPort(std::string destination);

private:
    std::vector<std::string> destAddr;
    std::vector<std::string> subnetMask;
    std::vector<std::string> gateWayMask;
    std::vector<int> interfacePort;
    std::vector<int> metric;
    std::vector<std::string> protocol;


};

#endif // ROUTINGTABLE_H
