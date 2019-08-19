//
// Created by Valpha on 2019/8/19.
//

#ifndef RPCTUNER_COMWITHDRIVERBASE_H
#define RPCTUNER_COMWITHDRIVERBASE_H

#include "../../RPC_GLOBAL.h"

class ComWithDriver {
public:
    virtual void InitHardware(void) = 0;

    virtual RPC_ERRCODE openHardware(void) = 0;

    virtual void closeHardware(void) = 0;

    virtual void writeBytes(uByte *data, uByte length) = 0;

    static void (*pGetDataCallback)(uByte *data, uByte length);

    static uByte *outPutData;
};


#endif //RPCTUNER_COMWITHDRIVERBASE_H
