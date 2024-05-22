#ifndef ROUTER_H
#define ROUTER_H

#include "node.h"
#include <string>

#include <QObject>

class Router : public Node
{
    Q_OBJECT
public:
    explicit Router(QObject *parent = nullptr);
    void createPacket();
    void send(std::shared_ptr<std::string> data);
public slots:
    void recievePacket(std::shared_ptr<std::string> data);

signals:
    void sendPacket(std::shared_ptr<std::string> data);
};

#endif // ROUTER_H
