#include "node.h"

Node::Node(int _id,QObject *parent)
    : QObject{parent}
{
    id = _id;
}



