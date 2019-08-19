//
// Created by Valpha on 2019/8/19.
//

#include "ComWithDriver.h"

void (*ComWithDriver::pGetDataCallback)(uByte *data, uByte length) = nullptr;

uByte *ComWithDriver::outPutData = nullptr;
