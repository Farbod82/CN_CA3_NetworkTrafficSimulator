#include "routingtable.h"
#include <limits>
#include <queue>
#include <vector>
#include <stack>
#include <iostream>
#include <iomanip>

RoutingTable::RoutingTable(std::string _ip, QObject* parrent)
    :QObject{parrent}{
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

bool RoutingTable::updateRowBaseOneDestinationAndProtocol(std::string dest, std::string subMask,
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


void RoutingTable::updateRoutingTableOSPF(const LSDB& lsdb, int inport){
    QHash<std::string, std::pair<std::string, int>> StepCost = dijkstra(lsdb);

    for (auto key : StepCost.keys()){
        if (StepCost[key].second == std::numeric_limits<int>::max()){
            continue;
        }
        bool updated = false;
        for (int i = 0; i < destAddr.size(); ++i) {
            if (destAddr[i].compare(key) == 0
                && protocol[i] == "OSPF"){
                updateRowBaseOneDestinationAndProtocol(
                    key,
                    subnetMask[i],
                    StepCost[key].first,
                    interfacePort[i],
                    StepCost[key].second,
                    protocol[i]
                    );
                updated = true;
                break;
            }
        }

        if (!updated){
            insertRow(
                key,
                "",
                StepCost[key].first,
                inport,
                StepCost[key].second,
                "OSPF"
                );
        }
    }

}

std::ostream& operator<< (std::ostream& stream, const RoutingTable& rt) {
    // Print table header
    stream << "Routing Table for Router: " << rt.routerIp << std::endl;
    stream << std::left << std::setw(20) << "Destination" << std::left << std::setw(20) << "Subnet Mask"
           << std::left << std::setw(20) << "Gateway" << std::left << std::setw(15) << "Port"
           << std::left << std::setw(10) << "Metric" << std::left << std::setw(20) << "Protocol" << std::endl;
    stream << std::string(100, '-') << std::endl;

    // Print each row of data
    for (int i = 0; i < rt.destAddr.size(); ++i) {
        stream << std::left << std::setw(20) << rt.destAddr[i]
               << std::left << std::setw(20) << rt.subnetMask[i]
               << std::left << std::setw(20) << rt.gateWayMask[i]
               << std::left << std::setw(15) << rt.interfacePort[i]
               << std::left << std::setw(10) << rt.metric[i]
               << std::left << std::setw(20) << rt.protocol[i] << std::endl;
    }

    return stream;
}
