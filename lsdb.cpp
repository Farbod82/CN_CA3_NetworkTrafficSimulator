#include "lsdb.h"
#include <QObject>

LSDB::LSDB(std::string _routerIp){
    routerIp = _routerIp;
}

void LSDB::Dijkstra(){

}

void LSDB::updateByOspfPacket(OspfPacket* ospfPacket){
    if (lastSequenceNumber.contains(ospfPacket->getSource())){
        if (ospfPacket->getSequence() <= lastSequenceNumber[ospfPacket->getSource()]){
            return;
        }
    }
    Link links = ospfPacket->getLinks();
    QList<std::string> linkDests = links.keys();
    for (int i =0 ; i <linkDests.size(); i++){
        cost[ospfPacket->getSource()][linkDests[i]] = links[linkDests[i]];
        cost[linkDests[i]][ospfPacket->getSource()] = links[linkDests[i]];
    }
}

QHash<std::string, int> LSDB::operator[] (const std::string& a) const{
    return cost[a];
}


QList<std::string> LSDB::keys() const{
    return cost.keys();
}
