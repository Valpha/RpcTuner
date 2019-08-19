//
// Created by Valpha on 2019/8/19.
//

#ifndef RPCTUNER_CRPCLISTENER_H
#define RPCTUNER_CRPCLISTENER_H

#include <RPC_GLOBAL.h>

class CRpcListener {
public:
    int CRpcLisener();

    uByte modelcode;

    virtual void onReceivedInfo(uByte *pInfo, uByte length) = 0;
};


#endif //RPCTUNER_CRPCLISTENER_H
