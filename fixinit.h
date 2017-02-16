#ifndef FIXINIT_H
#define FIXINIT_H
#include "FixApi.h"
class FixProcess{
public:
    virtual BOOL initFixInterface();
    virtual void createAbossSystemConnect();
    HANDLE_CONN systemConnect;
};

#endif // FIXINIT_H
