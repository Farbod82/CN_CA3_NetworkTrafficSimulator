#ifndef NODE_H
#define NODE_H

#include "packet.h"
#include <QObject>
#include <string>

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(int _id,QObject *parent = nullptr);

public slots:


private:
    int id;

signals:
    void PacketToPort1(std::shared_ptr<Packet>);
    void PacketToPort2(std::shared_ptr<Packet>);
    void PacketToPort3(std::shared_ptr<Packet>);
    void PacketToPort4(std::shared_ptr<Packet>);
    void PacketToPort5(std::shared_ptr<Packet>);
};

#endif // NODE_H
