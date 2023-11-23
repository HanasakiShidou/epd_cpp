
#include "npi_neo3.hpp"

NPiNeo3_Soft_SPI::NPiNeo3_Soft_SPI(int pwr, int din, int clk, int cs, int dc, int rst, int busy)
				:EPD_IO{
					SysfsGPIO(pwr, OUT),
					SysfsGPIO(din, OUT),
					SysfsGPIO(clk, OUT),
					SysfsGPIO(cs, OUT),
					SysfsGPIO(dc, OUT), 
					SysfsGPIO(rst, OUT), 
					SysfsGPIO(busy, IN)} {

	spi.RegisterGPIO(SPI_PIN::SCK,
			[]() -> int { return 0;}, // Do not manage gpio in SPI object
			[this]() -> int { return EPD_IO[EPD_IO_PIN::CLK].Read();},
			[this](int value) -> int { return EPD_IO[EPD_IO_PIN::CLK].Write(value);},
			[]() -> int { return 0;} // Do not manage gpio in SPI object
	);
	
	spi.RegisterGPIO(SPI_PIN::MOSI,
		[]() -> int { return 0;}, // Do not manage gpio in SPI object
		[this]() -> int { return EPD_IO[EPD_IO_PIN::DIN].Read();},
		[this](int value) -> int { return EPD_IO[EPD_IO_PIN::DIN].Write(value);},
		[]() -> int { return 0;} // Do not manage gpio in SPI object
	);

	spi.RegisterGPIO(SPI_PIN::MISO,
		[]() -> int { return 0;}, // Do not manage gpio in SPI object
		[this]() -> int { return 0;},
		[this](int value) -> int { return 0;},
		[]() -> int { return 0;} // Do not manage gpio in SPI object
	);

	EPD_IO[EPD_IO_PIN::CS].Write(1);
	EPD_IO[EPD_IO_PIN::PWR].Write(1);
}

NPiNeo3_Soft_SPI::~NPiNeo3_Soft_SPI() {
	EPD_IO[EPD_IO_PIN::CS].Write(0);
	EPD_IO[EPD_IO_PIN::PWR].Write(0);
	EPD_IO[EPD_IO_PIN::DC].Write(0);
	EPD_IO[EPD_IO_PIN::RST].Write(0);
}

int NPiNeo3_Soft_SPI::DigitalWrite(EPD_IO_PIN pin, uint8_t value) {
	return EPD_IO[pin].Write(value);
}

uint8_t NPiNeo3_Soft_SPI::DigitalRead(EPD_IO_PIN pin) {
	return EPD_IO[pin].Read();
}

int NPiNeo3_Soft_SPI::SPIWriteByte(uint8_t value) {
	return spi.Transfer(value);
}

int NPiNeo3_Soft_SPI::SPIWriteBytes(uint8_t *pData, uint32_t len) {
	return 0;
}

int NPiNeo3_Soft_SPI::Delay(uint32_t ms) {
	for(uint32_t i = 0; i < ms; i++) {
		usleep(1000);
	}
	return 0;
	
}

int NPiNeo3_Soft_SPI::Init() {
	spi.SetBitOrder(SOFTWARE_SPI_Order::SOFTWARE_SPI_MSBFIRST);
	spi.SetDataMode(SOFTWARE_SPI_Mode::SOFTWARE_SPI_Mode0);
	spi.SetClockDivider(SOFTWARE_SPI_Clock::SOFTWARE_SPI_CLOCK_DIV4);

	if (!Available())
		return -1;
	return 0;
}

bool NPiNeo3_Soft_SPI::Available() {
	for (auto& io: EPD_IO) {
		if (!io.Available())
			return false;
	}
	return true;
}