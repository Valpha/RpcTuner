//
// Created by Valpha on 2019/8/20.
//

#ifndef RPCTUNER_CTUNERAPP_H
#define RPCTUNER_CTUNERAPP_H

#include <RPC_GLOBAL.h>
#include "../RpcApp/CRpcApp.h"

class CTunerRpcListener : public CRpcListener {
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

    RPC_ERRCODE registeTunerRPCLisiner(CTunerRpcListener *listener);

public:
    CTunerRpcListener *tunerRpcListener;
};

class CFMRadioRpcListener : public CTunerRpcListener {
    void onDataChanged(uByte *pInfo, uByte length) override;

public:
    CFMRadioRpcListener();

    virtual ~CFMRadioRpcListener();

    static pthread_mutex_t mutex;
    static uByte rpcdata[RPC_CMD_LENGTH];
    static bool rpcdataokflag;
};

#endif //RPCTUNER_CTUNERAPP_H
