#ifndef PACKET_H
#define PACKET_H


#include<string>
#include <vector>

class Packet
{
public:
    Packet(std::string _dest_addr, std::string _source_addr, std::string Type);
    void setNextHopPort(int _port);
    std::string getBody();
    void setBody(std::string _body);
    std::string getType();
    std::string getSource();
    void addASNumber(int _AS);
    int getInitialASNumber();


    virtual ~Packet(){}
    std::string getDest();
private:
    std::string Type;
    std::vector<int> ASNumbers;
    std::string dest_addr;
    std::string source_addr;
    int nextHopPort;
    std::string body;
};

#endif // PACKET_H
