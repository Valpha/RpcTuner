//
// Created by Valpha on 2019/8/19.
//


#include "UartDriver.h"

#define LOGD(...) LOGTAG("UartDriver", __VA_ARGS__)

Long UartDriver::uartfd = -1;
pthread_t UartDriver::rpc_receThreadID = 0;
pthread_mutex_t  UartDriver::mutex = PTHREAD_MUTEX_INITIALIZER;

UartDriver::UartDriver() {

    uartfd = -1;
}

UartDriver::~UartDriver() {

}

void UartDriver::InitHardware(void) {

}

RPC_ERRCODE UartDriver::openHardware(void) {
    LOGD("openHardware: enter\n");
    uartfd = openUart();
    if (uartfd > 0) {
        LOGD("openHardware: open Uart OK\n");
        pthread_create(&rpc_receThreadID, NULL, receiveThread, NULL);
        return RPC_OK;
    } else {
        LOGD("openHardware: false\n");
        return RPC_COM_ERROR;
    }
}

void UartDriver::closeHardware(void) {
    if (uartfd > 0) {
        close(uartfd);
    }
}

void UartDriver::writeBytes(uByte *data, uByte length) {
    pthread_mutex_lock(&mutex);
    uByte buff[MaxLength + 4];
    memset(buff, 0, sizeof(buff));
    memcpy(buff, data, length);
    LOGD("writeBytes: Writing...\n");
    for (uByte i = 0; i < length; i++) {
        LOGD("%X, ", data[i]);
    }
    LOGD("\n");
    buff[MaxLength] = 0x0D;
    buff[MaxLength + 1] = 0x0A;
    buff[MaxLength + 2] = 0x0D;
    buff[MaxLength + 3] = 0x0A;
    write(uartfd, buff, MaxLength + 4);
    pthread_mutex_unlock(&mutex);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void *UartDriver::receiveThread(void *args) {
    uByte nbytes = 0;
    uByte buff[MaxLength + 2];
    uByte *bufptr = buff;
    uByte length = 0;

    while (1) {
        LOGD("receiveThread: readBytes start...\n");
        memset(buff, 0, sizeof(buff));
        bufptr = buff;
        length = 0;
        if ((nbytes = read(uartfd, bufptr, buff + sizeof(buff) - bufptr - 1)) > 0) {
            LOGD("receiveThread: read %d data...\n", nbytes);
            bufptr += nbytes;
            length += nbytes;
            if (bufptr[-1] == 0x0D || bufptr[-1] == 0x0A) {
                bufptr[-1] = '\0';
            } else {
                tcflush(uartfd, TCIFLUSH);
                continue;
            }
        } else {
            tcflush(uartfd, TCIFLUSH);
            continue;
        }
        pthread_mutex_lock(&mutex);
        if (length > 1 && pGetDataCallback != NULL) {
            if (length > MaxLength) {
                length = MaxLength;
            }
            outPutData = (uByte *) malloc(MaxLength);
            memcpy(outPutData, buff, MaxLength);
//            memcpy(CRpcProtocol::mProtolData, buff, MaxLength);
            tcflush(uartfd, TCIFLUSH);
            LOGD("pGetDataCallBack: start...\n");
            pGetDataCallback(outPutData, length);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

#pragma clang diagnostic pop

Long UartDriver::openUart(void) {
    LOGD("openUart: Start...\n");
    uartfd = open(DEVICE, O_RDWR);
    if (uartfd < 0) {
        perror(DEVICE);
        return uartfd;
    }
    LOGD("openUart: setUartSpeed...\n");
    setUartSpeed(uartfd, BAUDRATE);
    if (setUartParity(uartfd, DATABITS, STOPBITS, PARITY) == FALSE) {
        LOGD("openUart: Set Parity Error\n");
        return uartfd;
    }
    LOGD("openUart: Success\n");
    return uartfd;
}
///////////////////////////////////////////////////////////////////
/**
*@brief  设置串口通信速率
*@param  fd     类型 int  打开串口的文件句柄
*@param  speed  类型 int  串口速度
*@return  void
*/
int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,
                   B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,};
int name_arr[] = {115200, 38400, 19200, 9600, 4800, 2400, 1200, 300,
                  115200, 38400, 19200, 9600, 4800, 2400, 1200, 300,};

void UartDriver::setUartSpeed(int fd, int speed) {
    uWord i;
    Long status;
    struct termios Opt;
    tcgetattr(fd, &Opt);
    for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
        if (speed == name_arr[i]) {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&Opt, speed_arr[i]);
            cfsetospeed(&Opt, speed_arr[i]);
            status = tcsetattr(fd, TCSANOW, &Opt);
            if (status != 0) {
                perror("tcsetattr fd1");
                return;
            }
            tcflush(fd, TCIOFLUSH);
        }
    }
}
///////////////////////////////////////////////////////////////////
/**
*@brief   设置串口数据位，停止位和效验位
*@param  fd     类型  int  打开的串口文件句柄
*@param  databits 类型  int 数据位   取值 为 7 或者8
*@param  stopbits 类型  int 停止位   取值为 1 或者2
*@param  parity  类型  int  效验类型 取值为N,E,O,,S
*/
Long UartDriver::setUartParity(int fd, int databits, int stopbits, int parity) {
    struct termios options;
    if (tcgetattr(fd, &options) != 0) {
        perror("SetupSerial 1");
        return (FALSE);
    }
    options.c_cflag &= ~CSIZE;
    switch (databits) /*设置数据位数*/
    {
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr, "Unsupported data size\n");
            return (FALSE);
    }
    switch (parity) {
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB;   /* Clear parity enable */
            options.c_iflag &= ~INPCK;     /* Enable parity checking */
            break;
        case 'o':
        case 'O':
            options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
            options.c_iflag |= INPCK;             /* Disnable parity checking */
            break;
        case 'e':
        case 'E':
            options.c_cflag |= PARENB;     /* Enable parity */
            options.c_cflag &= ~PARODD;   /* 转换为偶效验*/
            options.c_iflag |= INPCK;       /* Disnable parity checking */
            break;
        case 'S':
        case 's':  /*as no parity*/
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
            fprintf(stderr, "Unsupported parity\n");
            return (FALSE);
    }
    /* 设置停止位*/
    switch (stopbits) {
        case 1:
            options.c_cflag &= ~CSTOPB;
            break;
        case 2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            fprintf(stderr, "Unsupported stop bits\n");
            return (FALSE);
    }
    /* Set input parity option */
    if (parity != 'n')
        options.c_iflag |= INPCK;

    /*bug modify ref https://www.cnblogs.com/zgq0/p/8078505.html*/
    LOGD("setUartParity: Set options OK\n");
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_cflag |= (CLOCAL | CREAD);//添加的
    options.c_oflag &= ~(ONLCR | OCRNL); //添加的
    options.c_iflag &= ~(ICRNL | INLCR);//添加的
    options.c_iflag &= ~(IXON | IXOFF | IXANY); //添加的

    tcflush(fd, TCIFLUSH);
    options.c_cc[VTIME] = 150; /* 设置超时15 seconds*/
    options.c_cc[VMIN] = 0; /* Update the options and do it NOW */
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        perror("SetupSerial 3");
        return (FALSE);
    }

    return (TRUE);
}
