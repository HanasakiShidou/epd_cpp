
#pragma once

#include <unistd.h>
#include <errno.h>

#include "../hardwareapi.hpp"
#include "../../Tools/software_spi/sysfs_software_spi.hpp"
#include "../../Tools/sysfs_gpio/sysfs_gpio.hpp"

class NPiNeo3_Soft_SPI : public HardwareAPI 
{
public:
    NPiNeo3_Soft_SPI() = delete;
    NPiNeo3_Soft_SPI(int pwr, int din, int clk, int cs, int dc, int rst, int busy);
    ~NPiNeo3_Soft_SPI();
    int DigitalWrite(EPD_IO_PIN pin, uint8_t value) override;
    uint8_t DigitalRead(EPD_IO_PIN pin) override;
    int SPIWriteByte(uint8_t value) override;
    int SPIWriteBytes(uint8_t *pData, uint32_t len) override;
    int Delay(uint32_t ms) override;
    int Init() override;
    bool Available() override;
private:
    SysfsGPIO EPD_IO[EPD_IO_PIN::_LEN];

    SoftSPI spi;
};
