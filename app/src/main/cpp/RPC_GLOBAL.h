//
// Created by Valpha on 2019/8/20.
//

#ifndef RPCTUNER_RPC_GLOBAL_H
#define RPCTUNER_RPC_GLOBAL_H

#include    <pthread.h>
#include    <unistd.h>
#include    <pthread.h>
#include    <memory.h>
#include    <android/log.h>
#define LOGTAG(...) __android_log_print(ANDROID_LOG_DEBUG, __VA_ARGS__)

class Lock {
private:
    pthread_mutex_t m_lock;
public:
    Lock(pthread_mutex_t cs) : m_lock(cs) {
        pthread_mutex_lock(&m_lock);
    }

    ~Lock() {
        pthread_mutex_unlock(&m_lock);
    }
};

typedef enum {
    RPC_OK,
    RPC_PARA_ERROR,
    RPC_COM_ERROR
} RPC_ERRCODE;
typedef unsigned char uByte;
typedef unsigned short uWord;
typedef unsigned long uLong;
typedef signed long Long;
typedef unsigned long long uLongLong;
#define RPC_CMD_LENGTH 16
#endif //RPCTUNER_RPC_GLOBAL_H
