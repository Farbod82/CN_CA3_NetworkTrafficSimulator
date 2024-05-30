#include "clockgenerator.h"
#include <QThread>


clockGenerator::clockGenerator(QObject *parent)
    : QObject{parent}
{}


void clockGenerator::startGeneration(){
    while(true){
        QThread::msleep(1000);
        emit clockSignal();
    }
}
