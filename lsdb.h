#ifndef LSDB_H
#define LSDB_H

#include <QObject>
#include "ospfpacket.h"

typedef QHash<std::string, QHash<std::string, int> >  Edges;
typedef QHash<std::string, std::vector<std::string > > Path;
class LSDB: public QObject
{
    Q_OBJECT
public:
    explicit LSDB();
    void updateByOspfPacket(OspfPacket* ospfPacket);
    QHash<std::string, int> operator[] (const std::string& a) const;
    QList<std::string> keys() const;

private:
    std::string routerIp;
    Path pathToDestination;
    Edges cost;
    QHash<std::string, int> lastSequenceNumber;
    void removePreviousCosts(std::string dest);
};

// public slots:

#endif // LSDB_H
