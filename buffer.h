#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>
#include "packet.h"


class Buffer : public QObject
{
    Q_OBJECT
public:
    explicit Buffer(int _id ,QObject *parent = nullptr);
    void sendPacket();
    void addToOutBuffer(std::shared_ptr<Packet> packet);
    std::shared_ptr<Packet> getFirstPacket();
    void incWaitingCycles();
    void increaseDeliveryCycles();

private:
    int id;
    std::vector<std::shared_ptr<Packet>> inBuffer;
    std::vector<std::shared_ptr<Packet>> outBuffer;
public slots:
    void recievePacket(std::shared_ptr<Packet> packet);
signals:
    void sendPacketSignal(std::shared_ptr<Packet> packet);

};

#endif // BUFFER_H
