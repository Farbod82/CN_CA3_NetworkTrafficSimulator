#ifndef HOST_H
#define HOST_H

#include "buffer.h"
#include <QObject>
#include <random>

const int NUMBER_OF_PORTS = 5;

class Host : public QObject
{
    Q_OBJECT
public:
    explicit Host(std::string _ip, double parteo_alpha, double parteo_mx, QObject *parent = nullptr);
    void setPartners(std::vector<std::string>* _partners);
    void createAndSendPacket();
    double pareto_distribution();
    Buffer* port;

private:
    double alpha, xm;
    std::string ip;
    std::default_random_engine* generator;
    std::vector<std::string>* partners;
signals:


public slots:
    void parteoSendPacket();
    void receivePackets();
};

#endif // HOST_H
