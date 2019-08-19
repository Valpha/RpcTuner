//
// Created by Valpha on 2019/8/19.
//

#include "CRpcProtocol.h"

#define LOGD(...) LOGTAG("CRpcProtocol", __VA_ARGS__)

uByte CRpcProtocol::mProtolData[RPC_CMD_LENGTH] = {0};

CRpcProtocol::CRpcProtocol() {

}

void CRpcProtocol::setCheckSum(void) {
    uWord cksum = 0x0000;
    for (int i = 0; i < (RPC_CMD_LENGTH - 4); i++) {
        cksum += mProtolData[i];
    }
    mProtolData[15] = cksum & 0xFF;
    mProtolData[14] = (cksum >> 8) & 0xFF;
}

bool CRpcProtocol::checkCheckSum(uByte *pPpcData, uByte cmdLength) {
    if (NULL == pPpcData || cmdLength != RPC_CMD_LENGTH) {
        return false;
    }
    uWord realsum = mProtolData[15] + (mProtolData[14] << 8);
    uWord checksum = pPpcData[15] + (pPpcData[14] << 8);
    if (realsum == checksum) {
        return true;
    } else {
        return false;
    }
}

RPC_ERRCODE
CRpcProtocol::makeRpcCommond(uByte modelcode, uWord opcode, uByte *pCmdData, uByte cmdLength) {
    for (uByte i = 0; i < RPC_CMD_LENGTH; i++) {
        mProtolData[i] = 0;
    }

    mProtolData[0] = modelcode;
    mProtolData[1] = (opcode >> 8) & 0xFF;
    mProtolData[2] = opcode & 0xFF;
    if (cmdLength > (RPC_CMD_LENGTH - 5)) {
        return RPC_PARA_ERROR;
    }


    for (uByte i = 0; i < cmdLength; i++) {
        mProtolData[i + 3] = pCmdData[cmdLength - i - 1];
    }

    for (uByte i = 0; i < RPC_CMD_LENGTH; i++) {
        LOGD("%X, ", mProtolData[i]);
    }

    LOGD("\n");

    return RPC_OK;
}
