//
// Created by Valpha on 2019/8/19.
//

#ifndef RPCTUNER_CRPCAPP_H
#define RPCTUNER_CRPCAPP_H

#include    <vector>
#include <RPC_GLOBAL.h>
#include    "CRpcListener.h"
#include    "ComProxy/CRpcComProxy.h"
#include    "CRpcProtocol.h"

class CRpcApp {
public:
    static CRpcApp *getInstance(void);

private:
    CRpcApp();

private:
    static pthread_mutex_t mutex;
    static CRpcApp *pInstance;
    static RPC_ERRCODE rpcreadyFlag;
public:
    RPC_ERRCODE sendCommand(uByte modelcode, uWord opcode, uByte *pCmdData, uByte cmdLength);

    RPC_ERRCODE registerRpcLisener(CRpcListener *lisener);

    RPC_ERRCODE unregisterAllRpcLisener();

    RPC_ERRCODE unregisterRpcLisener(CRpcListener *lisener);

    RPC_ERRCODE getInfo(uByte *info, uByte length);

    static void rpcGetDataCallback(uByte *data, uByte length);

private:
    std::vector<CRpcListener *> listenerList;

};

#endif //RPCTUNER_CRPCAPP_H
