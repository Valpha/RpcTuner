//
// Created by Valpha on 2019/8/20.
//

#ifndef RPCTUNER_CTUNERAPP_H
#define RPCTUNER_CTUNERAPP_H

#include "../RpcApp/CRpcListener.h"

class CTunerRPCLisiner : public CRpcListener {
    virtual void onReceivedInfo(uByte *pInfo, uByte length);

    virtual void onDataChanged(uByte *pInfo, uByte length) = 0;
};

class CTunerApp {
private:
    CTunerApp();

public:
    ~CTunerApp();

    static CTunerApp *getInstance(void);

private:
    static pthread_mutex_t mutex;
    static CTunerApp *pInstance;

public:
    RPC_ERRCODE setFrequence(uLongLong frequence);

    RPC_ERRCODE seekUp(uLongLong frequence);

    RPC_ERRCODE seekDown(uLongLong frequence);

    RPC_ERRCODE getInfo(uByte *info, uByte length);

    RPC_ERRCODE registeTunerRPCLisiner(CTunerRPCLisiner *lisiner);

public:
    CTunerRPCLisiner *tunerRpcLisiner;
};


#endif //RPCTUNER_CTUNERAPP_H
