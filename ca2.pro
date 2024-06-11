QT = core

CONFIG += c++17 cmdline

QT += concurrent

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        buffer.cpp \
        clockgenerator.cpp \
        cluster.cpp \
        commandreader.cpp \
        host.cpp \
        ebgppacket.cpp \
        ibgppacket.cpp \
        lsdb.cpp \
        main.cpp \
        node.cpp \
        ospfpacket.cpp \
        packet.cpp \
        packetsaver.cpp \
        rippacket.cpp \
        router.cpp \
        routingtable.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    buffer.h \
    clockgenerator.h \
    cluster.h \
    commandreader.h \
    host.h \
    ebgppacket.h \
    ibgppacket.h \
    lsdb.h \
    node.h \
    ospfpacket.h \
    packet.h \
    packetsaver.h \
    rippacket.h \
    router.h \
    routingtable.h
