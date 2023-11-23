#pragma once

#include <cstdint>
#include <unistd.h>
#include <errno.h>
#include "../Config/Debug.h"

typedef enum {
    PWR,
    DIN,
    CLK,
    CS,
    DC,
    RST,
    BUSY,
    _LEN
} EPD_IO_PIN;

class HardwareAPI
{
public:
    virtual int DigitalWrite(EPD_IO_PIN pin, uint8_t value) = 0;
    virtual uint8_t DigitalRead(EPD_IO_PIN pin) = 0;
    virtual int SPIWriteByte(uint8_t value) = 0;
    virtual int SPIWriteBytes(uint8_t *pData, uint32_t len) = 0;
    virtual int Delay(uint32_t ms) = 0;
    virtual int Init() = 0;
    virtual bool Available() = 0;
    //virtual void Exit() = 0;
};
