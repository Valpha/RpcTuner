//
// Created by Valpha on 2019/8/19.
//

#include "CRpcApp.h"

#define LOGD(...) LOGTAG("CRpcApp", __VA_ARGS__)

using namespace std;

pthread_mutex_t  CRpcApp::mutex = PTHREAD_MUTEX_INITIALIZER;
CRpcApp *CRpcApp::pInstance = NULL;
RPC_ERRCODE CRpcApp::rpcreadyFlag = RPC_COM_ERROR;


CRpcApp *CRpcApp::getInstance(void) {
    LOGD("getInstance: enter\n");
    if (pInstance == NULL) {
        //double check
        LOGD("getInstance: lock\n");
        Lock lock(mutex);    //用lock实现线程安全
        if (pInstance == NULL) {
            LOGD("getInstance: new CRpcApp\n");
            pInstance = new CRpcApp();
        }
    }
    LOGD("getInstance: exit\n");
    return pInstance;
}

CRpcApp::CRpcApp() {
    LOGD("New CRpcApp begin \n");
    if (RPC_OK == CRpcComProxy::getInstance()->openHardware()) {
        LOGD("CRpcComProxy was ready!\n");
        CRpcComProxy::getInstance()->registGetDataCallback(rpcGetDataCallback);
        rpcreadyFlag = RPC_OK;
    } else {
        LOGD("CRpcComProxy was NOT ready!\n");

        rpcreadyFlag = RPC_COM_ERROR;
    }
    LOGD("New CRpcApp finished \n");
}

RPC_ERRCODE CRpcApp::sendCommand(uByte modelcode, uWord opcode, uByte *pCmdData, uByte cmdLength) {
    LOGD("sendCommand: enter\n");

    if (rpcreadyFlag != RPC_OK) {
        LOGD("sendCommand: RpcComProxy was NOT READY!!!\n");
        LOGD("sendCommand: try to call CRpcComProxy openHardware\n");
        RPC_ERRCODE errcode = CRpcComProxy::getInstance()->openHardware();
        if (RPC_OK == errcode) {
            LOGD("sendCommand: RPC_OK\n");
            rpcreadyFlag = RPC_OK;
        } else {
            LOGD("sendCommand: RPC_COM_ERROR\n");
            return RPC_COM_ERROR;
        }
    }
    CRpcProtocol::makeRpcCommond(modelcode, opcode, pCmdData, cmdLength);
    //TODO use uart or spi Driver to send rpc data to MCU
//    LOGD("sendCommand: writeBytes %X\n", CRpcProtocol::mProtolData[3]);
//    LOGD("sendCommand: writeBytes %X\n", CRpcProtocol::mProtolData[3]);
    CRpcComProxy::getInstance()->writeBytes(CRpcProtocol::mProtolData, RPC_CMD_LENGTH);
    return RPC_OK;
}

RPC_ERRCODE CRpcApp::registerRpcLisener(CRpcListener *lisener) {
    listenerList.push_back(lisener);
    return RPC_OK;
}

RPC_ERRCODE CRpcApp::unregisterAllRpcLisener() {
    for (vector<CRpcListener *>::iterator it = listenerList.begin(); it != listenerList.end();) {
        listenerList.erase(it);
        ++it;
    }
    return RPC_OK;
}

RPC_ERRCODE CRpcApp::unregisterRpcLisener(CRpcListener *lisener) {
    for (vector<CRpcListener *>::iterator it = listenerList.begin(); it != listenerList.end();) {
        listenerList.erase(it);
        ++it;
    }
    return RPC_OK;
}

RPC_ERRCODE CRpcApp::getInfo(uByte *info, uByte length) {
    //TODO send Cmd to MCU
//     CRpcComProxy::getInstance()->readBytes(info, length);
    return RPC_OK;
}

void CRpcApp::rpcGetDataCallback(uByte *data, uByte length) {
    LOGD("rpcGetDataCallback: enter");
    for (vector<CRpcListener *>::iterator it = pInstance->listenerList.begin();
         it != pInstance->listenerList.end();) {
        (*it)->onReceivedInfo(data, length);
        ++it;
        LOGD("rpcGetDataCallback: this CRpcListener was called!\n");
    }
    LOGD("rpcGetDataCallback: end");

}
