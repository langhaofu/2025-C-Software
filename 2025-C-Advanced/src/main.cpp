#include <iostream>
#include <string>
#include "pic_deal.h"
#include "thread_deal.h"
#include <opencv2/opencv.hpp>

// 声明识别正方形的函数
void shibie_Square_min(const cv::Mat& image, cv::Mat& result_image, std::vector<cv::Point2f>& min_square);

int main(int argc, char** argv) {
    std::cout << "程序启动: 正方形识别与最小正方形检测" << std::endl;

    // 初始化图像处理类
    PicDeal pic_deal;

    // 从摄像头读取图像
    if (!pic_deal.readFromCamera(0)) {
        std::cerr << "无法打开摄像头 /dev/video0" << std::endl;
        return -1;
    }
    std::cout << "摄像头已打开，开始处理图像..." << std::endl;

    // 创建线程池
    ThreadPool thread_pool(4);

    // 主循环
    bool running = true;
    while (running) {
        // 获取当前图像
        cv::Mat frame = pic_deal.getCurrentImage();
        if (frame.empty()) {
            std::cerr << "无法获取图像帧" << std::endl;
            break;
        }

        // 创建结果图像
        cv::Mat result_image = frame.clone();

        // 存储最小正方形的顶点
        std::vector<cv::Point2f> min_square;

        // 使用线程池处理图像
        thread_pool.enqueue([&frame, &result_image, &min_square]() {
            // 调用正方形识别函数
            shibie_Square_min(frame, result_image, min_square);
        });

        // 等待任务完成
        thread_pool.waitForCompletion();

        // 显示结果
        cv::imshow("原始图像", frame);
        cv::imshow("识别结果", result_image);

        // 显示最小正方形信息
        if (!min_square.empty()) {
            std::cout << "找到最小正方形，边长: ";
            // 计算边长
            float edge_length = cv::norm(min_square[0] - min_square[1]);
            std::cout << edge_length << " 像素" << std::endl;
        }

        // 按下 'q' 键退出
        if (cv::waitKey(1) == 'q') {
            running = false;
        }
    }

    std::cout << "程序退出" << std::endl;
    cv::destroyAllWindows();
    return 0;
}