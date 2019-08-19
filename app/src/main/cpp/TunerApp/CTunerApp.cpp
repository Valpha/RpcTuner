//
// Created by Valpha on 2019/8/20.
//


#include "CTunerApp.h"

#define LOGD(...) LOGTAG("CTunerApp", __VA_ARGS__)

pthread_mutex_t  CTunerApp::mutex = PTHREAD_MUTEX_INITIALIZER;
CTunerApp *CTunerApp::pInstance = NULL;

CTunerApp::CTunerApp() {}

CTunerApp::~CTunerApp() {

}

CTunerApp *CTunerApp::getInstance(void) {
    LOGD("getInstance: enter\n");
    if (pInstance == NULL) {
        //double check
        LOGD("getInstance: lock\n");
        Lock lock(mutex);    //用lock实现线程安全
        if (pInstance == NULL) {
            LOGD("getInstance: new CTunerApp\n");
            pInstance = new CTunerApp();
        }
    }
    LOGD("getInstance: exit\n");
    return pInstance;
}

RPC_ERRCODE CTunerApp::setFrequence(uLongLong frequence) {
    LOGD("setFrequence: enter\n");
    if (frequence < 87500 || frequence > 108000) {
        return RPC_PARA_ERROR;
    }
    uByte pCmdData[3] = {0};
    pCmdData[0] = frequence & 0xFF;
    pCmdData[1] = (frequence >> 8) & 0xFF;
    pCmdData[2] = (frequence >> 16) & 0xFF;

    CRpcApp::getInstance()->sendCommand(0x01, 0x0001, pCmdData, sizeof(pCmdData));
    LOGD("setFrequence: exit\n");
    return RPC_OK;
}

RPC_ERRCODE CTunerApp::seekUp(uLongLong frequence) {
    LOGD("setFrequence: enter\n");
    if (frequence < 87500 || frequence > 108000) {
        return RPC_PARA_ERROR;
    }
    uByte pCmdData[3] = {0};
    pCmdData[0] = frequence & 0xFF;
    pCmdData[1] = (frequence >> 8) & 0xFF;
    pCmdData[2] = (frequence >> 16) & 0xFF;

    CRpcApp::getInstance()->sendCommand(0x01, 0x0002, pCmdData, sizeof(pCmdData));
    return RPC_OK;
}

RPC_ERRCODE CTunerApp::seekDown(uLongLong frequence) {
    LOGD("setFrequence: enter\n");
    if (frequence < 87500 || frequence > 108000) {
        return RPC_PARA_ERROR;
    }
    uByte pCmdData[3] = {0};
    pCmdData[0] = frequence & 0xFF;
    pCmdData[1] = (frequence >> 8) & 0xFF;
    pCmdData[2] = (frequence >> 16) & 0xFF;
    CRpcApp::getInstance()->sendCommand(0x01, 0x0004, pCmdData, sizeof(pCmdData));
    return RPC_OK;
}

RPC_ERRCODE CTunerApp::getInfo(uByte *info, uByte length) {
    CRpcApp::getInstance()->getInfo(info, length);
    return RPC_OK;
}

RPC_ERRCODE CTunerApp::registeTunerRPCLisiner(CTunerRpcListener *listener) {
    tunerRpcListener = listener;
    return CRpcApp::getInstance()->registerRpcLisener(listener);

}

void CTunerRpcListener::onReceivedInfo(uByte *pInfo, uByte length) {
    LOGD("%d\n", length);
    for (int i = 0; i < length; i++) {
        LOGD("%X, ", pInfo[i]);
    }
    LOGD("\n");
    if (pInfo != NULL) {
        uWord opcode = (pInfo[1] << 8) | pInfo[2];
        LOGD("opcode %d\n", opcode);
        LOGD("pInfo[0] %d\n", pInfo[0]);
        if (pInfo[0] == 0x01) {
            if (CTunerApp::getInstance()->tunerRpcListener != NULL) {
                CTunerApp::getInstance()->tunerRpcListener->onDataChanged(pInfo, length);
            }
        }
    }
}

pthread_mutex_t  CFMRadioRpcListener::mutex = PTHREAD_MUTEX_INITIALIZER;
uByte CFMRadioRpcListener::rpcdata[RPC_CMD_LENGTH];
bool CFMRadioRpcListener::rpcdataokflag = false;

void CFMRadioRpcListener::onDataChanged(uByte *pInfo, uByte length) {
    if (CFMRadioRpcListener::rpcdataokflag == false) {
        for (uByte i = 0; i < length; i++) {
            LOGD("%X ", pInfo[i]);
            CFMRadioRpcListener::rpcdata[i] = pInfo[i];
        }
        CFMRadioRpcListener::rpcdataokflag = true;
    }
}

CFMRadioRpcListener::CFMRadioRpcListener() {}

CFMRadioRpcListener::~CFMRadioRpcListener() {

}
