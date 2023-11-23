#pragma once

#include <cstdint>
#include <cstdio>
#include <functional>
#include <memory>

#define SYSFS_SOFTWARE_SPI_DEBUG 1
#if SYSFS_SOFTWARE_SPI_DEBUG
#define SYSFS_SOFTWARE_SPI_Debug(__info,...) printf("Debug: " __info,##__VA_ARGS__)
#else
#define SYSFS_SOFTWARE_SPI_Debug(__info,...)
#endif

/**
 * SPI communication mode
**/
typedef enum {
    SOFTWARE_SPI_Mode0,   /* Clock Polarity is 0 and Clock Phase is 0 */
    SOFTWARE_SPI_Mode1,   /* Clock Polarity is 0 and Clock Phase is 1 */
    SOFTWARE_SPI_Mode2,   /* Clock Polarity is 1 and Clock Phase is 0 */
    SOFTWARE_SPI_Mode3,   /* Clock Polarity is 1 and Clock Phase is 1 */
} SOFTWARE_SPI_Mode;

/**
 * SPI clock(div)
**/
typedef enum {
    SOFTWARE_SPI_CLOCK_DIV2,
    SOFTWARE_SPI_CLOCK_DIV4,
    SOFTWARE_SPI_CLOCK_DIV8,
    SOFTWARE_SPI_CLOCK_DIV16,
    SOFTWARE_SPI_CLOCK_DIV32,
    SOFTWARE_SPI_CLOCK_DIV64,
    SOFTWARE_SPI_CLOCK_DIV128,
} SOFTWARE_SPI_Clock;

/**
 * Define SPI type
**/
typedef enum {
    SOFTWARE_SPI_Master,
    SOFTWARE_SPI_Slave,
} SOFTWARE_SPI_Type;

/**
 * Define SPI order
**/
typedef enum {    
    SOFTWARE_SPI_LSBFIRST,
    SOFTWARE_SPI_MSBFIRST,
} SOFTWARE_SPI_Order;

/**
 * Define GPIO for SPI
**/
typedef enum {
    SCK,
    MOSI,
    MISO,
    _LIST_LEN
} SPI_PIN;

//const std::function<int(int)> dummy_int_call = [](int) -> int {0;};
//const std::function<int(void)> dummy_void_call = []() -> int {0;};

class SoftSPI{
    public:
        SoftSPI();
        ~SoftSPI();
        void Begin();
        void End();
        int SetBitOrder(uint8_t order);
        int SetDataMode(SOFTWARE_SPI_Mode mode);
        int SetBitOrder(SOFTWARE_SPI_Order order);
        int SetClockDivider(SOFTWARE_SPI_Clock div);
        int Transfer(uint8_t value);

        int RegisterGPIO(SPI_PIN pin,
                         const std::function<int(void)> init,
                         const std::function<int(void)> read,
                         const std::function<int(int)> write,
                         const std::function<int(void)> close);
    private:
        //Mode
        struct GPIO_Operation{
            //GPIO_Operation():init(dummy_void_call), read(dummy_void_call), write(dummy_int_call), close(dummy_void_call) {}
            GPIO_Operation() = default;
            GPIO_Operation(const std::function<int(void)> init_,
                           const std::function<int(void)> read_,
                           const std::function<int(int)> write_,
                           const std::function<int(void)> close_):
                           init(init_), read(read_), write(write_), close(close_) {}
            const std::function<int(void)> init;
            const std::function<int(void)> read;
            const std::function<int(int)> write;
            const std::function<int(void)> close;
        };

        std::unique_ptr<GPIO_Operation> GPIO[SPI_PIN::_LIST_LEN];

        SOFTWARE_SPI_Mode mode_;
        uint8_t CPOL;
        uint8_t CPHA;

        uint8_t delay_;
        SOFTWARE_SPI_Type type_;
        SOFTWARE_SPI_Order order_;
};
