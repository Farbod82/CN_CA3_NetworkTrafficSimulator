#ifndef COMMANDREADER_H
#define COMMANDREADER_H

#include "packet.h"
#include "packetsaver.h"

#include <QObject>

class CommandReader : public QObject
{
    Q_OBJECT

public:
    explicit CommandReader(QObject *parent = nullptr);
    void setPacketSaver(PacketSaver *_packetSaver);
    void printAllPackets();
    void printPacket(std::string packetNum);
signals:
    void printRoutingTableRequested(std::string Command);

public slots:
    void readCommands();

private:
    PacketSaver* packetSaver;
    bool checkPrintRoutingTable(const std::string &command);

};

#endif // COMMANDREADER_H
