#include "uart.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

UART::UART() : serial_fd_(-1), is_open_(false) {
    // 构造函数初始化
}

UART::~UART() {
    // 析构函数关闭串口
    if (is_open_) {
        close();
    }
}

bool UART::init(const std::string& port, uint32_t baud_rate, uint8_t parity, uint8_t data_bits, uint8_t stop_bits) {
    // 关闭已打开的串口
    if (is_open_) {
        close();
    }

    // 打开串口设备
    serial_fd_ = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial_fd_ < 0) {
        std::cerr << "Failed to open port " << port << ": " << strerror(errno) << std::endl;
        return false;
    }

    // 设置非阻塞模式
    fcntl(serial_fd_, F_SETFL, 0);

    // 获取当前串口配置
    struct termios options;
    tcgetattr(serial_fd_, &options);

    // 设置波特率
    speed_t baud_rate_code;
    switch (baud_rate) {
        case 9600:   baud_rate_code = B9600;   break;
        case 19200:  baud_rate_code = B19200;  break;
        case 38400:  baud_rate_code = B38400;  break;
        case 57600:  baud_rate_code = B57600;  break;
        case 115200: baud_rate_code = B115200; break;
        default:     baud_rate_code = B9600;   break;
    }
    cfsetispeed(&options, baud_rate_code);
    cfsetospeed(&options, baud_rate_code);

    // 设置数据位
    options.c_cflag &= ~CSIZE;
    switch (data_bits) {
        case 5: options.c_cflag |= CS5; break;
        case 6: options.c_cflag |= CS6; break;
        case 7: options.c_cflag |= CS7; break;
        case 8: default: options.c_cflag |= CS8; break;
    }

    // 设置校验位
    switch (parity) {
        case 1:  // 奇校验
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            break;
        case 2:  // 偶校验
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            break;
        case 0:  // 无校验
        default:
            options.c_cflag &= ~PARENB;
            break;
    }

    // 设置停止位
    if (stop_bits == 2) {
        options.c_cflag |= CSTOPB;
    } else {
        options.c_cflag &= ~CSTOPB;
    }

    // 设置其他选项
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 0;

    // 应用配置
    tcsetattr(serial_fd_, TCSANOW, &options);

    is_open_ = true;
    return true;
}

int UART::send(const uint8_t* data, size_t len) {
    if (!is_open_ || serial_fd_ < 0) {
        std::cerr << "UART not initialized" << std::endl;
        return -1;
    }

    int bytes_written = write(serial_fd_, data, len);
    if (bytes_written < 0) {
        std::cerr << "Failed to write to UART: " << strerror(errno) << std::endl;
        return -1;
    }

    return bytes_written;
}

int UART::receive(uint8_t* buffer, size_t len, int timeout_ms) {
    if (!is_open_ || serial_fd_ < 0) {
        std::cerr << "UART not initialized" << std::endl;
        return -1;
    }

    // 设置超时
    struct timeval tv;
    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(serial_fd_, &read_fds);

    int ret = select(serial_fd_ + 1, &read_fds, NULL, NULL, &tv);
    if (ret < 0) {
        std::cerr << "Select error: " << strerror(errno) << std::endl;
        return -1;
    } else if (ret == 0) {
        // 超时
        return 0;
    }

    // 有数据可读
    int bytes_read = read(serial_fd_, buffer, len);
    if (bytes_read < 0) {
        std::cerr << "Failed to read from UART: " << strerror(errno) << std::endl;
        return -1;
    }

    return bytes_read;
}

void UART::close() {
    if (serial_fd_ >= 0) {
        ::close(serial_fd_);
        serial_fd_ = -1;
    }
    is_open_ = false;
}

bool UART::isOpen() const {
    return is_open_;
}