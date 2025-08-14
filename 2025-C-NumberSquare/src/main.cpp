#include <iostream>
#include <string>
#include "python_wrapper.h"

int main(int argc, char* argv[]) {
    // 初始化Python解释器
    if (!initPython()) {
        std::cerr << "Failed to initialize Python interpreter" << std::endl;
        return 1;
    }

    // 创建Python包装器
    PythonWrapper wrapper;

    // 加载模型
    if (!wrapper.loadModel("../models/best_epoch_weights.pth")) {
        std::cerr << "Failed to load model" << std::endl;
        cleanupPython();
        return 1;
    }

    std::cout << "模型加载成功，等待图像输入..." << std::endl;

    // 主循环
    std::string imagePath;
    while (true) {
        std::cout << "请输入图像路径 (输入'q'退出): ";
        std::getline(std::cin, imagePath);

        if (imagePath == "q" || imagePath == "Q") {
            break;
        }

        // 识别图像中的正方形
        std::string result = wrapper.recognizeSquare(imagePath);
        std::cout << "识别结果: " << result << std::endl;
    }

    // 清理Python解释器
    cleanupPython();

    return 0;
}