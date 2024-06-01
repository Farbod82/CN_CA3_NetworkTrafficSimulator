#ifndef COMMANDREADER_H
#define COMMANDREADER_H

#include <QObject>

class CommandReader : public QObject
{
    Q_OBJECT

public:
    explicit CommandReader(QObject *parent = nullptr);

signals:
    void printRoutingTableRequested(std::string Command);

public slots:
    void readCommands();

private:
    bool checkPrintRoutingTable(const std::string &command);
};

#endif // COMMANDREADER_H
