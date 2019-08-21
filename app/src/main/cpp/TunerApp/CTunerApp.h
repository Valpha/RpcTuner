//
// Created by Valpha on 2019/8/20.
//

#ifndef RPCTUNER_CTUNERAPP_H
#define RPCTUNER_CTUNERAPP_H

#include <RPC_GLOBAL.h>
#include "../RpcApp/CRpcApp.h"

class CTunerRpcListener : public CRpcListener {
    void onReceivedInfo(uByte *pInfo, uByte length) override;

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
    RPC_ERRCODE setFrequency(uLong frequency);

    RPC_ERRCODE seekUp(uLong frequence);

    RPC_ERRCODE seekDown(uLong frequence);

    RPC_ERRCODE getInfo(uByte *info, uByte length);

    RPC_ERRCODE registeTunerRPCLisiner(CTunerRpcListener *listener);

    RPC_ERRCODE unregisteTunerRPCLisiner(CTunerRpcListener *listener);



public:
    CTunerRpcListener *tunerRpcListener;
};

class CFMRadioRpcListener : public CTunerRpcListener {
    void onDataChanged(uByte *pInfo, uByte length) override;

public:
    CFMRadioRpcListener();

    virtual ~CFMRadioRpcListener();

    static pthread_mutex_t mutex;
    uByte rpcData[RPC_CMD_LENGTH];
    bool rpcDataOkFlag;
};

#endif //RPCTUNER_CTUNERAPP_H
