TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fixinit.cpp
LIBS += -L/home/tff/software/develop/build-dingdian-Desktop-Debug/config -lfixapi_x64
HEADERS += \
    FixApi_mdb.h \
    FixApi.h \
    fixinit.h \
    blsdef_3rd.h \
    fiddef.h
