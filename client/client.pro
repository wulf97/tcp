TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/Client.cpp \
    src/TAGPackage.cpp \
    src/LongInt.tpp \
    src/main.cpp

HEADERS += \
    src/Client.h \
    src/TAGPackage.h \
    src/TextColor.h \
    src/LongInt.h
