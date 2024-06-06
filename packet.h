#ifndef PACKET_H
#define PACKET_H


#include<string>
#include <vector>

typedef enum IPVersion{
    IPV_4,
    IPV_6
} IPVersion;

class Packet
{
public:
    Packet(std::string _dest_addr, std::string _source_addr, std::string Type, IPVersion _ipv = IPV_4);
    void setNextHopPort(int _port);
    std::string getBody();
    void setBody(std::string _body);
    std::string getType();
    std::string getSource();
    void addASNumber(int _AS);
    bool changeToIPV6();
    IPVersion getIpVersion();
    void increaseDeliveryCycles();
    void increaseBufferWaitingCycles();    int getInitialASNumber();


    virtual ~Packet(){}
    std::string getDest();
protected:
    std::string source_addr;

private:
    std::string Type;
    std::vector<int> ASNumbers;
    std::string dest_addr;
    int nextHopPort;
    std::string body;
    int bufferWaitingCycles = 0;
    int deliveryCycles = 0;
    std::vector<std::string> route;
    IPVersion ipv;
    Packet* ipv6Packet = nullptr;
    bool encapsulated = false;

};

#endif // PACKET_H
