#include "commandreader.h"
#include <QThread>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

CommandReader::CommandReader(QObject *parent) : QObject(parent)
{
}

void CommandReader::readCommands()
{
    while (true)
    {
        std::string command;
        std::cout << "Enter a command: ";
        std::getline(std::cin, command);

        istringstream iss(command);
        string s;
        vector<std::string> commandSplits;
        while ( getline( iss, s, ' ' ) ) {
            commandSplits.push_back(s);
        }
        if (commandSplits.empty()){
            continue;
        }
        std::cout <<commandSplits[0] <<std::endl;
        if(commandSplits[0].compare("print") == 0)
            emit printRoutingTableRequested(commandSplits[1]);
        QThread::sleep(1);
    }
}

bool CommandReader::checkPrintRoutingTable(const std::string &command)
{
    return command.compare("print") == 0;
}
