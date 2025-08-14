#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

/**
 * 识别图像中的正方形，处理重叠情况，并找出最小的正方形
 * @param image 输入图像
 * @param result_image 输出结果图像
 * @param min_square 输出最小正方形的顶点
 */
void shibie_Square_min(const cv::Mat& image, cv::Mat& result_image, std::vector<cv::Point2f>& min_square) {
    // 创建灰度图像
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // 高斯模糊
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

    // 边缘检测
    cv::Mat edges;
    cv::Canny(blurred, edges, 50, 150);

    // 寻找轮廓
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 存储所有检测到的正方形
    std::vector<std::vector<cv::Point2f>> squares;

    // 处理每个轮廓
    for (size_t i = 0; i < contours.size(); i++) {
        // 多边形近似
        std::vector<cv::Point> approx;
        double epsilon = 0.04 * cv::arcLength(contours[i], true);
        cv::approxPolyDP(contours[i], approx, epsilon, true);

        // 检查是否为四边形
        if (approx.size() == 4 && cv::isContourConvex(approx)) {
            // 计算边长
            std::vector<double> edge_lengths;
            for (int j = 0; j < 4; j++) {
                int next_idx = (j + 1) % 4;
                double length = cv::norm(approx[j] - approx[next_idx]);
                edge_lengths.push_back(length);
            }

            // 排序边长
            std::sort(edge_lengths.begin(), edge_lengths.end());

            // 检查是否为正方形（四边长度相近）
            double max_diff = edge_lengths[3] - edge_lengths[0];
            if (max_diff < 0.1 * edge_lengths[0]) {
                // 计算轮廓面积
                double area = cv::contourArea(approx);
                double perimeter = cv::arcLength(approx, true);
                double circularity = 4 * CV_PI * area / (perimeter * perimeter);

                // 正方形的circularity约为0.785
                if (circularity > 0.7 && circularity < 0.85) {
                    // 转换为Point2f
                    std::vector<cv::Point2f> square;
                    for (const auto& point : approx) {
                        square.push_back(cv::Point2f(point));
                    }
                    squares.push_back(square);

                    // 在结果图像上绘制轮廓
                    cv::polylines(result_image, approx, true, cv::Scalar(0, 255, 0), 2);
                }
            }
        }
    }

    // 处理重叠正方形
    // 这里简化处理，仅保留不重叠或面积较大的正方形
    std::vector<std::vector<cv::Point2f>> non_overlapping_squares;
    for (size_t i = 0; i < squares.size(); i++) {
        bool is_overlapping = false;
        double area_i = cv::contourArea(squares[i]);

        for (size_t j = 0; j < squares.size(); j++) {
            if (i == j) continue;

            double area_j = cv::contourArea(squares[j]);
            // 如果j的面积比i大很多，且i在j内部，则认为i是j的内部轮廓
            if (area_j > 1.5 * area_i) {
                // 检查i是否在j内部
                bool all_inside = true;
                for (const auto& point : squares[i]) {
                    double dist = cv::pointPolygonTest(squares[j], point, false);
                    if (dist < 0) {
                        all_inside = false;
                        break;
                    }
                }
                if (all_inside) {
                    is_overlapping = true;
                    break;
                }
            }
        }

        if (!is_overlapping) {
            non_overlapping_squares.push_back(squares[i]);
        }
    }

    // 找出最小的正方形
    if (!non_overlapping_squares.empty()) {
        size_t min_index = 0;
        double min_area = cv::contourArea(non_overlapping_squares[0]);

        for (size_t i = 1; i < non_overlapping_squares.size(); i++) {
            double area = cv::contourArea(non_overlapping_squares[i]);
            if (area < min_area) {
                min_area = area;
                min_index = i;
            }
        }

        // 存储最小正方形
        min_square = non_overlapping_squares[min_index];

        // 在结果图像上绘制最小正方形（红色）
        std::vector<cv::Point> min_square_points;
        for (const auto& point : min_square) {
            min_square_points.push_back(cv::Point(point));
        }
        cv::polylines(result_image, min_square_points, true, cv::Scalar(0, 0, 255), 3);

        // 在最小正方形旁边显示"最小"字样
        cv::Point text_pos = min_square_points[0] - cv::Point(10, 10);
        cv::putText(result_image, "最小", text_pos, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 2);
    }
}