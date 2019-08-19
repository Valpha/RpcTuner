//
// Created by Valpha on 2019/8/19.
//

#ifndef RPCTUNER_CRPCCOMPROXY_H
#define RPCTUNER_CRPCCOMPROXY_H

#include "ComWithDriver.h"
#include "UartDriver.h"

class CRpcComProxy : public ComWithDriver {
public:
    static CRpcComProxy *getInstance(uByte senderType = 0);

    void InitHardware(void) override;

    RPC_ERRCODE openHardware(void) override;

    void closeHardware(void) override;

    void writeBytes(uByte *data, uByte length) override;

    void registGetDataCallback(void (*pGetDataCallback)(uByte *data, uByte length));

    void unregistGetDataCallback(void);


private:
    CRpcComProxy(ComWithDriver *sender);

    static pthread_mutex_t mutex;
    static CRpcComProxy *pInstance;
    ComWithDriver *m_sender;
};


#endif //RPCTUNER_CRPCCOMPROXY_H
