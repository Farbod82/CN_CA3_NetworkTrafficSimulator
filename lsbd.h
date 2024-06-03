#ifndef LSBD_H
#define LSBD_H

#include <QObject>
#include "ospfpacket.h"

typedef QHash<std::string, QHash<std::string, int> >  Edges;
typedef QHash<std::string, std::vector<std::string > > Path;
class LSDB: public QObject
{
    Q_OBJECT
public:
    explicit LSDB(std::string);
    void updateByOspfPacket(OspfPacket* ospfPacket);

private:
    std::string routerIp;
    Path pathToDestination;
    Edges cost;
    QHash<std::string, int> lastSequenceNumber;
    void Dijkstra();
};

// public slots:

#endif // LSBD_H
