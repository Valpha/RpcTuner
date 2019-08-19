//
// Created by Valpha on 2019/8/19.
//

#ifndef RPCTUNER_CUARTDRIVER_H
#define RPCTUNER_CUARTDRIVER_H

#include "ComWithDriver.h"
#include "UserComConfig.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define FALSE  -1
#define TRUE   0

/**
 * @brief   UARTs
 * @author  Valpha
 * @param MaxLength 数据长度
 * @param DEVICE    端口类型
 * @param BAUDRATE  串口速度，波特率
 * @param DATABITS  数据位长度，取值7或8
 * @param STOPBITS  截止位长度，取值1或2
 * @param PARITY    奇偶校验位，取值 N,E,O,,S (char)
 */
#define MaxLength  UART_MAXLENGTH
#define BAUDRATE   UART_BAUDRATE
#define DEVICE     UART_DEVICE
#define DATABITS   UART_DATABITS
#define STOPBITS   UART_STOPBITS
#define PARITY     UART_PARITY


class UartDriver : public ComWithDriver {
public:
    UartDriver();

    ~UartDriver();

    void InitHardware(void) override;

    RPC_ERRCODE openHardware(void) override;

    void closeHardware(void) override;

    void writeBytes(uByte *data, uByte length) override;

    static void *receiveThread(void *args);


private:
    // 串口操作函数
    Long openUart(void);

    void setUartSpeed(int fd, int speed);

    Long setUartParity(int fd, int databits, int stopbits, int parity);

    static Long uartfd;
    static pthread_t rpc_receThreadID;
    static pthread_mutex_t mutex;


};


#endif //RPCTUNER_CUARTDRIVER_H
