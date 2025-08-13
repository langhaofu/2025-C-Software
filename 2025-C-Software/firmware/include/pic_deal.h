#ifndef PIC_DEAL_H
#define PIC_DEAL_H

#include <opencv2/opencv.hpp>

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
     * 图像缩放
     * @param width 目标宽度
     * @param height 目标高度
     * @return 缩放后的图像
     */
    cv::Mat resize(int width, int height);

    /**
     * 获取当前处理的图像
     * @return 当前图像的引用
     */
    cv::Mat& getCurrentImage();

private:
    cv::Mat current_image_; // 当前处理的图像
};

#endif // PIC_DEAL_H