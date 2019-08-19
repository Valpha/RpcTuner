//
// Created by Valpha on 2019/8/19.
//

#include "CRpcComProxy.h"

#define LOGD(...) LOGTAG("CRpcComProxy",__VA_ARGS__)

pthread_mutex_t  CRpcComProxy::mutex = PTHREAD_MUTEX_INITIALIZER;
CRpcComProxy *CRpcComProxy::pInstance = 0;

CRpcComProxy *CRpcComProxy::getInstance(uByte senderType) {
    if (pInstance == NULL) {
        //double check
        Lock lock(mutex);    //用lock实现线程安全
        if (pInstance == NULL) {
            switch (senderType) {
                case 0:
                    pInstance = new CRpcComProxy(new UartDriver);
                    break;
                default:
                    break;
            }
        }
    }
    return pInstance;
}


CRpcComProxy::CRpcComProxy(ComWithDriver *sender) : m_sender(sender) {

}

void CRpcComProxy::InitHardware(void) {
    m_sender->InitHardware();

}

RPC_ERRCODE CRpcComProxy::openHardware(void) {
    return m_sender->openHardware();
}

void CRpcComProxy::closeHardware(void) {
    m_sender->closeHardware();
    LOGD("closeHardware: done\n");
}

void CRpcComProxy::writeBytes(uByte *data, uByte length) {
    LOGD("CRpcComProxy::writeBytes enter\n");
    m_sender->writeBytes(data, length);
    LOGD("writeBytes: exit\n");
}

void CRpcComProxy::registGetDataCallback(void (*pCallback)(uByte *, uByte)) {
    m_sender->pGetDataCallback = pCallback;
    LOGD("registGetDataCallback: done\n");
}

void CRpcComProxy::unregistGetDataCallback(void) {
    m_sender->pGetDataCallback = NULL;
    LOGD("unregistGetDataCallback: done\n");
}
