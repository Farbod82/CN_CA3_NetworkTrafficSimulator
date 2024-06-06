#ifndef LSDB_H
#define LSDB_H

#include <QObject>
#include "ospfpacket.h"

typedef QHash<std::string, QHash<std::string, int> >  Edges;
class LSDB: public QObject
{
    Q_OBJECT
public:
    explicit LSDB(QObject *parent = nullptr);
    void updateByOspfPacket(OspfPacket* ospfPacket);
    QHash<std::string, int> operator[] (const std::string& a) const;
    QList<std::string> keys() const;
    bool oldSequence(OspfPacket* ospfPacket);

private:
    Edges cost;
    QHash<std::string, int> lastSequenceNumber;
    void removePreviousCosts(std::string dest);
};

// public slots:

#endif // LSDB_H
