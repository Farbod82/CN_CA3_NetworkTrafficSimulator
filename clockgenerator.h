#ifndef CLOCKGENERATOR_H
#define CLOCKGENERATOR_H

#include <QObject>

class clockGenerator : public QObject
{
    Q_OBJECT
public:
    explicit clockGenerator(QObject *parent = nullptr);

    void startGeneration();
signals:
    void clockSignal();
};

#endif // CLOCKGENERATOR_H
