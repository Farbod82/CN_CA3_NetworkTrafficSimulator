#include "routingtable.h"
#include <limits>
#include <queue>
#include <vector>
#include <stack>
#include <iostream>

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

void RoutingTable::printShortestPath(const QHash<std::string, DJNode>& dist, std::string dest) {
    if (!dist.contains(dest)) {
        std::cout << "Destination " << dest << " not reachable from source" << std::endl;
        return;
    }

    std::stack<std::string> path;
    std::string current = dest;
    while (current != "") {
        path.push(current);
        current = dist[current].name;
    }

    std::cout << "Shortest path: ";
    while (!path.empty()) {
        std::cout << path.top() << " -> ";
        path.pop();
    }
    std::cout << "END" << std::endl;
}

std::pair<std::string, int> getFirstStepAndTotalCost(const QHash<std::string, DJNode>& dist, const std::string& dest) {
    if (dist.count(dest) == 0) {
        return {"", -1}; // Unreachable destination
    }
    return {dist[dest].parent, dist[dest].distance};
}

QHash<std::string, std::pair<std::string, int>> RoutingTable::dijkstra(const LSDB& lsdb) {
    QHash<std::string, DJNode> dist;
    std::priority_queue<std::pair<int, std::string>, std::vector<std::pair<int, std::string>>, std::greater<std::pair<int, std::string>>> pq;

    for (const auto& node : lsdb.keys()) {
        dist[node] = {node, std::numeric_limits<int>::max(), ""};
    }

    dist[routerIp].distance = 0;
    pq.push({0, routerIp});

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        std::string currentNode = pq.top().second;
        pq.pop();

        // Relaxation step (avoid processing already visited nodes)
        if (currentDistance > dist[currentNode].distance) {
            continue;
        }

        for (const auto& neighbor : lsdb[currentNode].keys()) {
            int edge = lsdb[currentNode][neighbor];
            int newDistance = currentDistance + edge;
            if (newDistance < dist[neighbor].distance) {
                dist[neighbor].distance = newDistance;
                dist[neighbor].parent = currentNode;
                pq.push({newDistance, neighbor});
            }
        }
    }

    QHash<std::string, std::pair<std::string, int>> results;
    for (const auto& node : dist.keys()) {
        results[node] = getFirstStepAndTotalCost(dist, node);
    }

    return results;
}


void RoutingTable::updateRoutingTableOSPF(LSDB* lsdb){
    QHash a<std::string, int>;
    std::string destination;
    for (int i = 0; i < destAddr.size(); i++) {
        destination = destAddr[i];
        if (lsdb[routerIp].contains(destination)){
            if (lsdb[routerIp][destination] < getDestinationCost(destination, "OSPF")){

            }
        }
    }

}
