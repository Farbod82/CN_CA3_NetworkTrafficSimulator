#include "routingtable.h"

RoutingTable::RoutingTable() {

    destAddr = new std::vector<>();
    subnectMask = new std::vector<>();
    gateWayMask = new std::vector<>();
    interfacePort = new std::vector<>();
    metric = new std::vector<>();
    protocol = new std::vector<>();
}

int RoutingTable::getOutputPort(std::string _destination, int _protocol){
    for (int i = 0; i < destAddr.size(); i++){
        if (destAddr[i] == _destination &&
            protocol[i] == _protocol){
            return interfacePort[i];
        }
    }
    
    return null;
}

bool RoutingTable::insertRow(std::string dest, std::string subMask,
              std::string gate, int port, int metr, std::string prot){
    if (getOutputPort(dest) != null){
        return false;
    }
    else{
        destAddr.push_back(dest);
        subnectMask.push_back(subMask);
        GateWayMask.push_back(gate);
        interfacePort.push_back(port);
        Metric.push_back(metr);
        Protocol.push_back(prot);
        return true;
    }
}

bool RoutingTable::editRowBaseOneDestinationAndProtocol(std::string dest, std::string subMask,
              std::string gate, int port, int metr, std::string prot){
    for (int i = 0; i < destAddr.size(); i++){
        if (destAddr[i] == destination &&
            protocol[i] == prot){
            subnectMask[i] = subMask;
            gateWayMask[i] = gate;
            interfacePort[i] = port;
            metric[i] = metr;
            return true;
        }
    }
    return false;
}


bool RoutingTable::removeRow(std::string _destination, int _protocol){
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

    return null;
}
