#include <iostream>
#include <string>
#include "pic_deal.h"
#include "uart.h"

int main(int argc, char** argv) {
    std::cout << "Firmware started" << std::endl;

    // 初始化UART
    UART uart;
    if (!uart.init("/dev/ttyS0", 115200)) {
        std::cerr << "Failed to initialize UART" << std::endl;
        return -1;
    }
    std::cout << "UART initialized successfully" << std::endl;

    // 初始化图像处理类
    PicDeal pic_deal;

    // 示例：读取图像
    // if (!pic_deal.readImage("image.jpg")) {
    //     std::cerr << "Failed to read image" << std::endl;
    //     uart.close();
    //     return -1;
    // }
    // std::cout << "Image read successfully" << std::endl;

    // 主循环
    bool running = true;
    while (running) {
        // 接收UART数据
        uint8_t buffer[128];
        int bytes_read = uart.receive(buffer, sizeof(buffer));

        if (bytes_read > 0) {
            std::cout << "Received data: ";
            for (int i = 0; i < bytes_read; ++i) {
                std::cout << std::hex << static_cast<int>(buffer[i]) << " ";
            }
            std::cout << std::dec << std::endl;

            // 简单的命令处理
            if (buffer[0] == 'q' || buffer[0] == 'Q') {
                running = false;
                std::cout << "Quitting..." << std::endl;
            }
            // 在这里可以添加更多命令处理逻辑
        }

        // 示例：图像处理
        // cv::Mat gray_img = pic_deal.toGrayscale();
        // cv::Mat binary_img = pic_deal.binarize(128);
        // pic_deal.saveImage("processed_image.jpg");

        // 短暂延时
        usleep(10000); // 10ms
    }

    // 清理资源
    uart.close();
    std::cout << "Firmware exited" << std::endl;

    return 0;
}