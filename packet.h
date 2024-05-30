#ifndef PACKET_H
#define PACKET_H


#include<string>

class Packet
{
public:
    Packet(std::string _dest_addr, std::string _source_addr, std::string Type);
    void setNextHopPort(int _port);
    std::string getBody();
    void setBody(std::string _body);
private:
    std::string Type;
    std::string dest_addr;
    std::string source_addr;
    int nextHopPort;
    std::string body;
};

#endif // PACKET_H
