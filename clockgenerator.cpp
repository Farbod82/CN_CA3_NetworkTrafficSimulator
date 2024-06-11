#include "clockgenerator.h"
#include <QThread>


clockGenerator::clockGenerator(QObject *parent)
    : QObject{parent}
{}


void clockGenerator::startGeneration(){
    while(true){
        QThread::msleep(100);
        emit clockSignal();
    }
}
