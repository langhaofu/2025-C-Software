#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <string>

/**
 * UART通信类
 * 负责串口的初始化、发送和接收数据
 */
class UART {
public:
    /**
     * 构造函数
     */
    UART();

    /**
     * 析构函数
     */
    ~UART();

    /**
     * 初始化串口
     * @param port 串口号
     * @param baud_rate 波特率
     * @param parity 校验位 (0:无校验, 1:奇校验, 2:偶校验)
     * @param data_bits 数据位 (5-8)
     * @param stop_bits 停止位 (1-2)
     * @return 是否初始化成功
     */
    bool init(const std::string& port, uint32_t baud_rate, uint8_t parity = 0, uint8_t data_bits = 8, uint8_t stop_bits = 1);

    /**
     * 发送数据
     * @param data 要发送的数据
     * @param len 数据长度
     * @return 实际发送的字节数
     */
    int send(const uint8_t* data, size_t len);

    /**
     * 接收数据
     * @param buffer 接收缓冲区
     * @param len 缓冲区长度
     * @param timeout_ms 超时时间(毫秒)
     * @return 实际接收的字节数
     */
    int receive(uint8_t* buffer, size_t len, int timeout_ms = 1000);

    /**
     * 关闭串口
     */
    void close();

    /**
     * 检查串口是否打开
     * @return 是否打开
     */
    bool isOpen() const;

private:
    int serial_fd_; // 串口文件描述符
    bool is_open_;  // 串口是否打开
};

#endif // UART_H