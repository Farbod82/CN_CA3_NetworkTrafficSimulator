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
    Packet(std::string _dest_addr, std::string _source_addr, std::string _mask, std::string Type, IPVersion _ipv = IPV_4);
    void setBody(std::string _body);
    void addASNumber(int _AS);
    void addPath(std::string _ip);
    bool changeToIPV6();
    void increaseDeliveryCycles();
    void increaseBufferWaitingCycles();    int getInitialASNumber();
    int getBufferCycles();
    int getDeliveryCycles();
    std::vector<std::string> getPath();
    std::string getType();
    std::string getSource();
    std::string getBody();
    std::string getDest();
    IPVersion getIpVersion();

    virtual ~Packet(){}
    void setPacketDestination(std::string _dest);
    void setFinalDest(std::string dest);
    std::string getFinalDest();
    void setType(std::string _type);
protected:
    std::string source_addr;
    std::vector<int> ASNumbers;
    std::vector<std::string> path;
    std::string finalDest;
    std::string mask;
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
