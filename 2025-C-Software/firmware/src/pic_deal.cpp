#include "pic_deal.h"
#include <iostream>

PicDeal::PicDeal() {
    // 构造函数初始化
}

PicDeal::~PicDeal() {
    // 析构函数清理
}

bool PicDeal::readImage(const std::string& img_path) {
    current_image_ = cv::imread(img_path);
    if (current_image_.empty()) {
        std::cerr << "Failed to read image: " << img_path << std::endl;
        return false;
    }
    return true;
}

bool PicDeal::saveImage(const std::string& img_path) {
    if (current_image_.empty()) {
        std::cerr << "No image to save" << std::endl;
        return false;
    }
    return cv::imwrite(img_path, current_image_);
}

cv::Mat PicDeal::toGrayscale() {
    if (current_image_.empty()) {
        std::cerr << "No image to process" << std::endl;
        return cv::Mat();
    }
    cv::Mat gray_img;
    cv::cvtColor(current_image_, gray_img, cv::COLOR_BGR2GRAY);
    return gray_img;
}

cv::Mat PicDeal::binarize(int threshold) {
    if (current_image_.empty()) {
        std::cerr << "No image to process" << std::endl;
        return cv::Mat();
    }
    cv::Mat gray_img = toGrayscale();
    cv::Mat binary_img;
    cv::threshold(gray_img, binary_img, threshold, 255, cv::THRESH_BINARY);
    return binary_img;
}

cv::Mat PicDeal::resize(int width, int height) {
    if (current_image_.empty()) {
        std::cerr << "No image to process" << std::endl;
        return cv::Mat();
    }
    cv::Mat resized_img;
    cv::resize(current_image_, resized_img, cv::Size(width, height));
    return resized_img;
}

cv::Mat& PicDeal::getCurrentImage() {
    return current_image_;
}