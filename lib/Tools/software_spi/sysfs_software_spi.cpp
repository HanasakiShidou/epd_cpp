#include "sysfs_software_spi.hpp"

static constexpr int HIGH = 1;
static constexpr int LOW = 0;

SoftSPI::SoftSPI() {
    Begin();
}

SoftSPI::~SoftSPI() {
    
}

int SoftSPI::RegisterGPIO(SPI_PIN pin,
                         const std::function<int(void)> init,
                         const std::function<int(void)> read,
                         const std::function<int(int)> write,
                         const std::function<int(void)> close) {
        GPIO[pin] = std::unique_ptr<GPIO_Operation>(new GPIO_Operation(init, read, write, close));
        return 0;
    }

void SoftSPI::Begin() {
    //software spi configure
    mode_ = SOFTWARE_SPI_Mode0;
    type_ = SOFTWARE_SPI_Master;
    delay_ = SOFTWARE_SPI_CLOCK_DIV2;
    order_ = SOFTWARE_SPI_MSBFIRST; // MSBFIRST
}

void SoftSPI::End() {
    for (int i = 0; i < SPI_PIN::_LIST_LEN; i++) {
        GPIO[i]->close();
    }
}

int SoftSPI::SetBitOrder(SOFTWARE_SPI_Order order) {
    order_ = order;
    return 0;
}

int SoftSPI::SetDataMode(SOFTWARE_SPI_Mode mode) {
    mode_ = mode;

    switch (mode) {
        case SOFTWARE_SPI_Mode0:
            CPOL = 0;
            CPHA = 0;
            break;
        case SOFTWARE_SPI_Mode1:
            CPOL = 0;
            CPHA = 1;
            break;
        case SOFTWARE_SPI_Mode2:
            CPOL = 1;
            CPHA = 0;
            break;
        case SOFTWARE_SPI_Mode3:
            CPOL = 1;
            CPHA = 1;
            break;
    }
    return 0;
}

int SoftSPI::SetClockDivider(SOFTWARE_SPI_Clock div) {
    switch (div) {
    case SOFTWARE_SPI_CLOCK_DIV2:
        delay_ = 2;
        break;
    case SOFTWARE_SPI_CLOCK_DIV4:
        delay_ = 4;
        break;
    case SOFTWARE_SPI_CLOCK_DIV8:
        delay_ = 8;
        break;
    case SOFTWARE_SPI_CLOCK_DIV16:
        delay_ = 16;
        break;
    case SOFTWARE_SPI_CLOCK_DIV32:
        delay_ = 32;
        break;
    case SOFTWARE_SPI_CLOCK_DIV64:
        delay_ = 64;
        break;
    case SOFTWARE_SPI_CLOCK_DIV128:
        delay_ = 128;
        break;
    default:
        delay_ = 128;
        break;
    }
    return 0;
}

int SoftSPI::Transfer(uint8_t value)
{
    uint8_t Read_data = 0;
    if (order_ == SOFTWARE_SPI_LSBFIRST) {
        uint8_t temp =
            ((value & 0x01) << 7) |
            ((value & 0x02) << 5) |
            ((value & 0x04) << 3) |
            ((value & 0x08) << 1) |
            ((value & 0x10) >> 1) |
            ((value & 0x20) >> 3) |
            ((value & 0x40) >> 5) |
            ((value & 0x80) >> 7);
        value = temp;
    }

    uint8_t delay = delay_ >> 1;
    for(int j=delay; j > 0; j--);

    uint8_t Read_miso = 0;
    
    GPIO[SPI_PIN::SCK]->write(LOW);
    //SYSFS_GPIO_Write(software_spi.SCLK_PIN, 0);
    for (uint8_t bit = 0; bit < 8; bit++) {     
        GPIO[SPI_PIN::SCK]->write(LOW);   
        //SYSFS_GPIO_Write(software_spi.SCLK_PIN, 0);
        // for(int j=delay; j > 0; j--);// DELAY

        if (CPHA) {
             Read_miso = GPIO[SPI_PIN::MISO]->read();
            //Read_miso = SYSFS_GPIO_Read(software_spi.MISO_PIN);
            if (order_ == SOFTWARE_SPI_LSBFIRST) {
                Read_data <<= 1;
                Read_data |= Read_miso;
            } else {
                Read_data >>= 1;
                Read_data |= Read_miso << 7;
            }
        } else {
            GPIO[SPI_PIN::MOSI]->write(((value<<bit) & 0x80) ? HIGH : LOW);
            //SYSFS_GPIO_Write(software_spi.MOSI_PIN, ((value<<bit) & 0x80) ? HIGH : LOW);
        }

        // for(int j=delay; j > 0; j--);// DELAY
        GPIO[SPI_PIN::SCK]->write(HIGH);
        //SYSFS_GPIO_Write(software_spi.SCLK_PIN, 1);
        // for(int j=delay; j > 0; j--);// DELAY

        if (CPHA) {
            GPIO[SPI_PIN::MOSI]->write(((value<<bit) & 0x80) ? HIGH : LOW);
            //SYSFS_GPIO_Write(software_spi.MOSI_PIN, ((value<<bit) & 0x80) ? HIGH : LOW);
        } else {
            Read_miso = GPIO[SPI_PIN::MISO]->read();
            //Read_miso = SYSFS_GPIO_Read(software_spi.MISO_PIN);
            if (order_ == SOFTWARE_SPI_LSBFIRST) {
                Read_data <<= 1;
                Read_data |= Read_miso;
            } else {
                Read_data >>= 1;
                Read_data |= Read_miso << 7;
            }
        }

        // for(int j=delay; j > 0; j--);// DELAY
    }
    return Read_data;
}
