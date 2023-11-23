#pragma once

#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>

constexpr int IN = 0;
constexpr int OUT = 1;

constexpr int LOW = 0;
constexpr int HIGH = 1;

//constexpr int NUM_MAXBUF = 4;
//constexpr int DIR_MAXSIZ = 60;

#define SYSFS_GPIO_DEBUG 0
#if SYSFS_GPIO_DEBUG 
	#define SYSFS_GPIO_Debug(__info,...) printf("Debug: " __info,##__VA_ARGS__)
#else
	#define SYSFS_GPIO_Debug(__info,...)  
#endif 

class SysfsGPIO
{
	private:
	int direction_;
	int pinNumber_;
	bool ready_;
	int fd_;

	public:
	SysfsGPIO(const SysfsGPIO&) = delete;
	SysfsGPIO& operator=(const SysfsGPIO&) = delete;
	SysfsGPIO() = delete;
	SysfsGPIO(int pinNumber, int direction): direction_(direction), pinNumber_(pinNumber) {
		ready_ = false;
		fd_ = -1;

		if (Export(pinNumber) < 0) {
			return;
		}

		if (SetDirection(pinNumber) < 0) {
			return;
		}
		ready_ = true;
	}

	~SysfsGPIO() {
		Unexport();
	}

	explicit operator bool() const{
		return ready_;
	}

	int Export(int pin) {
		int fd;
		int ret = -1;

		if (pin < 0) {
			return -1;
		}

		fd = open("/sys/class/gpio/export", O_WRONLY);
		if (fd < 0) {
			SYSFS_GPIO_Debug("Export Failed: Pin%d\n", pinNumber_);
		}
		else {
			std::string pinNumber(std::to_string(pinNumber_));
			write(fd, pinNumber.c_str(), pinNumber.size() + 1);
			SYSFS_GPIO_Debug( "Export: Pin%d\r\n", pinNumber_);
			ret = 1;
		}
		close(fd);
		return ret;
	}
	
	int Unexport(){
		Close();

		int fd;
		int ret = -1;

		fd = open("/sys/class/gpio/unexport", O_WRONLY);
		if (fd < 0) {
			SYSFS_GPIO_Debug("Unexport Failed: Pin%d\n", pinNumber_);
		}
		else {
			std::string pinNumber(std::to_string(pinNumber_));
			write(fd, pinNumber.c_str(), pinNumber.size() + 1);
			SYSFS_GPIO_Debug( "Unexport: Pin%d\r\n", pinNumber_);
			ret = 1;
		}
		close(fd);
		return ret;
	}

	int SetDirection(int pin) {
		const char dir_in[] = "in";
		const char dir_out[] = "out";

		std::stringstream path;
		path << "/sys/class/gpio/gpio" << pin << "/direction";

		int fd;

		fd = open(path.str().c_str(), O_WRONLY);
		if (fd < 0) {
			SYSFS_GPIO_Debug( "Set Direction failed: Pin%d\n", pin);
			return -1;
		}

		if (write(fd, direction_ == IN ? dir_in : dir_out, direction_ == IN ? 2 : 3) < 0) {
			SYSFS_GPIO_Debug("failed to set direction!\r\n");
			return -1;
		}

		if(direction_ == IN){
			SYSFS_GPIO_Debug("Pin%d:Input\r\n", pin);
		}else{
			SYSFS_GPIO_Debug("Pin%d:Output\r\n", pin);
		}
		close(fd);
		return 0;
	}

	const bool Available() {
		return ready_;
	}

	const bool ReadDirection(){
		return direction_;
	}

	int TryOpen() {
		if (!ready_) {
			return -1;
		} else if (fd_ < 0) {
			std::stringstream path;
			path << "/sys/class/gpio/gpio" << pinNumber_ << "/value";
			fd_ = open(path.str().c_str(), direction_ == IN ? O_RDONLY | O_SYNC : O_RDWR | O_SYNC);
			if (fd_ < 0) {
				SYSFS_GPIO_Debug( "Open failed on Pin%d\n", pinNumber_);
				ready_ = false;
				return -1;
			}
		}
		return fd_;
	}
	
	inline int Close() {
		if (fd_ >= 0) {
			return close(fd_);
		}
		return 0;
	}

	int Read() {
		if (TryOpen() < 0) {
			return -1;
		}

		char value_str[3];

		if (read(fd_, value_str, 3) < 0) {
			SYSFS_GPIO_Debug( "Read failed on Pin%d\n", pinNumber_);
			ready_ = false;
			return -1;
		} else {
			lseek(fd_, 0, SEEK_SET);
		}
		return atoi(value_str);
	}

	int Write(uint8_t value) {
		if (direction_ == OUT) {
			if (TryOpen() < 0) {
				SYSFS_GPIO_Debug("Try open failed\n");
				return -1;
			}

			const char value_str[] = "01";

			if (write(fd_, &value_str[value ? 1 : 0], 1) < 0) {
				SYSFS_GPIO_Debug( "Write failed on Pin%d\n", pinNumber_);
				ready_ = false;
				return -1;
			} else {
				lseek(fd_, 0, SEEK_SET);
			}
			return 0;
		} else {
			return -1;
		}
	}
};