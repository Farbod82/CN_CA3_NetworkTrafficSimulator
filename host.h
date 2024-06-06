#ifndef HOST_H
#define HOST_H

#include "buffer.h"
#include <QObject>
#include <random>

class Host : public QObject
{
    Q_OBJECT
public:
    explicit Host(std::string _ip, double parteo_alpha, double parteo_mx, int _as, QObject *parent = nullptr);
    void setPartners(const std::vector<std::string>& _partners);
    void createAndSendPacket();
    double pareto_distribution();
    Buffer* port;
    std::string getIp();
    bool regularType(Packet* packet);

private:
    double alpha, xm;
    std::string ip;
    std::default_random_engine* generator;
    std::vector<std::string> partners;
    int AS;
signals:


public slots:
    void parteoSendPacket();
    void handlePackets();
};

#endif // HOST_H
