TEMPLATE = app
CONFIG += console c++11
CONFIG -= qt

LIBS += \
    -lpthread

#LIBS += -lws2_32

SOURCES += \
    src/TAGPackage.cpp \
    src/main.cpp \
    src/Server.cpp \
    src/Session.cpp \
    src/LongInt.tpp

HEADERS += \
    src/Server.h \
    src/Session.h \
    src/TAGPackage.h \
    src/TextColor.h \
    src/LongInt.h

