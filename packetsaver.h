#ifndef PACKETSAVER_H
#define PACKETSAVER_H

#include "packet.h"
#include <QObject>

class PacketSaver : public QObject
{
    Q_OBJECT
public:
    explicit PacketSaver(QObject *parent = nullptr);
    std::vector<std::shared_ptr<Packet>> getSavedPackets();

private:
    std::vector<std::shared_ptr<Packet>> savedPackets;
signals:

public slots:
    void savePackets(std::shared_ptr<Packet> _packet);
};

#endif // PACKETSAVER_H
