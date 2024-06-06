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
    void setBody(std::string _body);
    void addASNumber(int _AS);
    bool changeToIPV6();
    void increaseDeliveryCycles();
    void increaseBufferWaitingCycles();    int getInitialASNumber();
    std::string getType();
    std::string getSource();
    std::string getBody();
    std::string getDest();
    IPVersion getIpVersion();

    virtual ~Packet(){}
    void setPacketDestination(std::string _dest);
protected:
    std::string source_addr;
    std::vector<int> ASNumbers;

private:
    std::string Type;
    std::string dest_addr;
    std::string body;
    int bufferWaitingCycles = 0;
    int deliveryCycles = 0;
    std::vector<std::string> route;
    IPVersion ipv;
    Packet* ipv6Packet = nullptr;
    bool encapsulated = false;

};

#endif // PACKET_H
