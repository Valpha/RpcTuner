//
// Created by Valpha on 2019/8/19.
//

#ifndef RPCTUNER_CRPCPROTOCOL_H
#define RPCTUNER_CRPCPROTOCOL_H

#include "../RPC_GLOBAL.h"

class CRpcProtocol {
public:
    CRpcProtocol();

public:
    static uByte mProtolData[RPC_CMD_LENGTH];

    static void setCheckSum(void);

    static bool checkCheckSum(uByte *pPpcData, uByte cmdLength);

    static RPC_ERRCODE
    makeRpcCommond(uByte modelcode, uWord opcode, uByte *pCmdData, uByte cmdLength);

};


#endif //RPCTUNER_CRPCPROTOCOL_H
