#ifndef PIC_DEAL_H
#define PIC_DEAL_H

#include <opencv2/opencv.hpp>
#include <vector>

/**
 * 图像处理类
 * 负责图像的读取、预处理、特征提取等操作
 */
class PicDeal {
public:
    /**
     * 构造函数
     */
    PicDeal();

    /**
     * 析构函数
     */
    ~PicDeal();

    /**
     * 读取图像
     * @param img_path 图像路径
     * @return 是否读取成功
     */
    bool readImage(const std::string& img_path);

    /**
     * 从摄像头读取图像
     * @param camera_id 摄像头ID
     * @return 是否读取成功
     */
    bool readFromCamera(int camera_id = 0);

    /**
     * 保存图像
     * @param img_path 保存路径
     * @return 是否保存成功
     */
    bool saveImage(const std::string& img_path);

    /**
     * 图像灰度化
     * @return 处理后的灰度图像
     */
    cv::Mat toGrayscale();

    /**
     * 图像二值化
     * @param threshold 阈值
     * @return 处理后的二值图像
     */
    cv::Mat binarize(int threshold);

    /**
     * 图像边缘检测
     * @param low_threshold 低阈值
     * @param high_threshold 高阈值
     * @return 边缘检测后的图像
     */
    cv::Mat edgeDetection(int low_threshold, int high_threshold);

    /**
     * 查找轮廓
     * @param contours 轮廓容器
     * @param hierarchy 轮廓层次结构
     * @return 是否查找成功
     */
    bool findContours(std::vector<std::vector<cv::Point>>& contours, std::vector<cv::Vec4i>& hierarchy);

    /**
     * 获取当前处理的图像
     * @return 当前图像的引用
     */
    cv::Mat& getCurrentImage();

private:
    cv::Mat current_image_; // 当前处理的图像
    cv::VideoCapture camera_; // 摄像头捕获对象
    bool is_camera_open_; // 摄像头是否打开
};

#endif // PIC_DEAL_H