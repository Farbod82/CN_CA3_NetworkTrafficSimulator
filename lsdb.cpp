#include "lsdb.h"
#include <QObject>

 void LSDB::removePreviousCosts(std::string dest){
    cost[dest].clear();
     for (auto d : cost[dest].keys()){
        if (cost[d].contains(dest)){
            cost[d].remove(dest);
        }
    }
}

void LSDB::updateByOspfPacket(OspfPacket* ospfPacket){
    if (lastSequenceNumber.contains(ospfPacket->getSource())){
        if (ospfPacket->getSequence() <= lastSequenceNumber[ospfPacket->getSource()]){
            return;
        }
    }

    removePreviousCosts(ospfPacket->getSource());
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
