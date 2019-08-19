//
// Created by Valpha on 2019/8/19.
//

#ifndef RPCTUNER_USERCOMCONFIG_H
#define RPCTUNER_USERCOMCONFIG_H

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
#define UART_MAXLENGTH  RPC_CMD_LENGTH
#define UART_BAUDRATE   115200
#define UART_DEVICE     "/dev/ttySAC1"
#define UART_DATABITS   8
#define UART_STOPBITS   1
#define UART_PARITY     'N'

#endif //RPCTUNER_USERCOMCONFIG_H
