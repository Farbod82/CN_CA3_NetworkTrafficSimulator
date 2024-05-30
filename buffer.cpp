#include "buffer.h"

Buffer::Buffer(int _id,QObject *parent)
    : QObject{parent}
{
    id = _id;
}

std::shared_ptr<Packet> Buffer::getFirstPacket(){
    if (!inBuffer.empty()){
        std::shared_ptr<Packet> poppedSharedPtr = inBuffer.back();
        inBuffer.pop_back();
        return poppedSharedPtr;
    }
    return nullptr;
}

void Buffer::recievePacket(std::shared_ptr<Packet> packet){
    inBuffer.insert(inBuffer.begin(), packet);
}

void Buffer::sendPacket(){
    if (!outBuffer.empty()){
        std::shared_ptr<Packet> poppedSharedPtr = outBuffer.back();
        outBuffer.pop_back();
        emit sendPacketSignal(poppedSharedPtr);
    }
}

void Buffer::addToOutBuffer(std::shared_ptr<Packet> packet){
    outBuffer.insert(outBuffer.begin(), packet);
}
