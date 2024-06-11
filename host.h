#ifndef HOST_H
#define HOST_H

#include "buffer.h"
#include <QObject>
#include <random>

class Host : public QObject
{
    Q_OBJECT
public:
    explicit Host(std::string _ip, double parteo_alpha, double parteo_mx, int _as, std::string _mask, QObject *parent = nullptr);
    void setPartners(const std::vector<std::string>& _partners);
    void createAndSendPacket();
    double pareto_distribution();
    Buffer* port;
    std::string getIp();
    bool regularType(Packet* packet);
    bool sent =0;

private:
    double alpha, xm;
    std::string ip;
    std::string mask;
    std::default_random_engine* generator;
    std::vector<std::string> partners;
    int AS;
signals:
    void sendPacket(std::shared_ptr<Packet> _packet);


public slots:
    void parteoSendPacket();
    void handlePackets();

};

#endif // HOST_H
