#include "pic_deal.h"
#include <iostream>

PicDeal::PicDeal() : is_camera_open_(false) {
    // 构造函数初始化
}

PicDeal::~PicDeal() {
    // 析构函数清理
    if (is_camera_open_) {
        camera_.release();
    }
}

bool PicDeal::readImage(const std::string& img_path) {
    current_image_ = cv::imread(img_path);
    if (current_image_.empty()) {
        std::cerr << "无法读取图像: " << img_path << std::endl;
        return false;
    }
    return true;
}

bool PicDeal::readFromCamera(int camera_id) {
    if (is_camera_open_) {
        camera_.release();
    }

    camera_.open(camera_id);
    if (!camera_.isOpened()) {
        std::cerr << "无法打开摄像头: " << camera_id << std::endl;
        is_camera_open_ = false;
        return false;
    }

    // 读取一帧图像以确保摄像头正常工作
    if (!camera_.read(current_image_)) {
        std::cerr << "无法从摄像头读取图像" << std::endl;
        camera_.release();
        is_camera_open_ = false;
        return false;
    }

    is_camera_open_ = true;
    return true;
}

bool PicDeal::saveImage(const std::string& img_path) {
    if (current_image_.empty()) {
        std::cerr << "没有图像可保存" << std::endl;
        return false;
    }
    return cv::imwrite(img_path, current_image_);
}

cv::Mat PicDeal::toGrayscale() {
    if (current_image_.empty()) {
        std::cerr << "没有图像可处理" << std::endl;
        return cv::Mat();
    }
    cv::Mat gray_img;
    cv::cvtColor(current_image_, gray_img, cv::COLOR_BGR2GRAY);
    return gray_img;
}

cv::Mat PicDeal::binarize(int threshold) {
    if (current_image_.empty()) {
        std::cerr << "没有图像可处理" << std::endl;
        return cv::Mat();
    }
    cv::Mat gray_img = toGrayscale();
    cv::Mat binary_img;
    cv::threshold(gray_img, binary_img, threshold, 255, cv::THRESH_BINARY);
    return binary_img;
}

cv::Mat PicDeal::edgeDetection(int low_threshold, int high_threshold) {
    if (current_image_.empty()) {
        std::cerr << "没有图像可处理" << std::endl;
        return cv::Mat();
    }
    cv::Mat gray_img = toGrayscale();
    cv::Mat blurred_img;
    cv::GaussianBlur(gray_img, blurred_img, cv::Size(3, 3), 0);
    cv::Mat edges;
    cv::Canny(blurred_img, edges, low_threshold, high_threshold);
    return edges;
}

bool PicDeal::findContours(std::vector<std::vector<cv::Point>>& contours, std::vector<cv::Vec4i>& hierarchy) {
    if (current_image_.empty()) {
        std::cerr << "没有图像可处理" << std::endl;
        return false;
    }

    cv::Mat edges = edgeDetection(50, 150);
    cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    return !contours.empty();
}

cv::Mat& PicDeal::getCurrentImage() {
    if (is_camera_open_) {
        camera_.read(current_image_);
    }
    return current_image_;
}