#include "routingtable.h"

RoutingTable::RoutingTable(std::string _ip){
    routerIp = _ip;
}

int RoutingTable::getOutputPort(std::string _destination, std::string _protocol){
    for (int i = 0; i < destAddr.size(); i++){
        if (destAddr[i] == _destination &&
            protocol[i] == _protocol){
            return interfacePort[i];
        }
    }
    
    return NO_WAY;
}

int RoutingTable::getDestinationCost(std::string _destination, std::string _protocol){
    for (int i = 0; i < destAddr.size(); i++){
        if (destAddr[i] == _destination &&
            protocol[i] == _protocol){
            return metric[i];
        }
    }

    return NO_WAY;
}

bool RoutingTable::insertRow(std::string dest, std::string subMask,
              std::string gate, int port, int metr, std::string prot){
    if (getOutputPort(dest, "OSPF") != NO_WAY){
        return false;
    }
    else{
        destAddr.push_back(dest);
        subnetMask.push_back(subMask);
        gateWayMask.push_back(gate);
        interfacePort.push_back(port);
        metric.push_back(metr);
        protocol.push_back(prot);
        return true;
    }
}

bool RoutingTable::editRowBaseOneDestinationAndProtocol(std::string dest, std::string subMask,
              std::string gate, int port, int metr, std::string prot){
    for (int i = 0; i < destAddr.size(); i++){
        if (destAddr[i] == dest &&
            protocol[i] == prot){
            subnetMask[i] = subMask;
            gateWayMask[i] = gate;
            interfacePort[i] = port;
            metric[i] = metr;
            return true;
        }
    }
    return false;
}


bool RoutingTable::removeRow(std::string _destination, std::string _protocol){
    for (int i = 0; i < destAddr.size(); i++){
        if (destAddr[i] == _destination &&
            protocol[i] == _protocol){
            subnetMask.erase(subnetMask.begin()+i);
            gateWayMask.erase(gateWayMask.begin()+i);
            interfacePort.erase(interfacePort.begin()+i);
            metric.erase(metric.begin()+i);
            return true;
        }
    }

    return NO_WAY;
}


// void RoutingTable::updateRoutingTableOSPF(LSDB* lsdb){
//     std::string destination;
//     for (int i = 0; i < destAddr.size(); i++) {
//         destination = destAddr[i];
//         if (lsdb[routerIp].containsKey(destination)){
//             if (lsdb[routerIp][destination] < getDestinationCost(destination, "OSPF")){

//             }
//         }
//     }

// }
