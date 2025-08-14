#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
相机标定脚本
用于标定相机内参和外参
参考文档: 9x7_35mm.pdf
"""

import numpy as np
import cv2
import os
import json

class CameraCalibrator:
    def __init__(self, square_size=0.035, pattern_size=(9, 7)):
        """
        初始化相机标定器
        :param square_size: 棋盘格每个方格的实际尺寸（米）
        :param pattern_size: 棋盘格的内角点数量 (宽度, 高度)
        """
        self.square_size = square_size
        self.pattern_size = pattern_size
        self.objpoints = []  # 3D世界坐标点
        self.imgpoints = []  # 2D图像坐标点
        self.camera_matrix = None
        self.dist_coeffs = None

    def prepare_object_points(self):
        """准备3D世界坐标点"""
        objp = np.zeros((self.pattern_size[0] * self.pattern_size[1], 3), np.float32)
        objp[:, :2] = np.mgrid[0:self.pattern_size[0], 0:self.pattern_size[1]].T.reshape(-1, 2)
        objp *= self.square_size
        return objp

    def find_chessboard_corners(self, image_path):
        """在图像中查找棋盘格内角点"""
        img = cv2.imread(image_path)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # 查找棋盘格内角点
        ret, corners = cv2.findChessboardCorners(gray, self.pattern_size, None)

        if ret:
            objp = self.prepare_object_points()
            self.objpoints.append(objp)

            # 亚像素级精确化
            criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
            corners2 = cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
            self.imgpoints.append(corners2)

            # 绘制并显示找到的角点
            img = cv2.drawChessboardCorners(img, self.pattern_size, corners2, ret)
            cv2.imshow('Chessboard Corners', img)
            cv2.waitKey(500)

        return ret

    def calibrate(self, image_dir):
        """使用指定目录下的图像进行相机标定"""
        # 获取所有图像路径
        image_paths = [os.path.join(image_dir, f) for f in os.listdir(image_dir) if f.endswith(('.jpg', '.png', '.jpeg'))]

        if not image_paths:
            print(f'在目录 {image_dir} 中未找到图像文件')
            return False

        # 查找所有图像中的棋盘格内角点
        for img_path in image_paths:
            print(f'处理图像: {img_path}')
            self.find_chessboard_corners(img_path)

        cv2.destroyAllWindows()

        # 进行相机标定
        img = cv2.imread(image_paths[0])
        h, w = img.shape[:2]
        ret, self.camera_matrix, self.dist_coeffs, _, _ = cv2.calibrateCamera(
            self.objpoints, self.imgpoints, (w, h), None, None)

        print(f'相机标定成功: {ret}')
        print('相机矩阵:\n', self.camera_matrix)
        print('畸变系数:\n', self.dist_coeffs)

        return ret

    def save_calibration_results(self, output_file):
        """保存标定结果到文件"""
        if self.camera_matrix is None or self.dist_coeffs is None:
            print('没有标定结果可保存')
            return False

        calibration_data = {
            'camera_matrix': self.camera_matrix.tolist(),
            'dist_coeffs': self.dist_coeffs.tolist()
        }

        with open(output_file, 'w') as f:
            json.dump(calibration_data, f, indent=4)

        print(f'标定结果已保存到 {output_file}')
        return True

if __name__ == '__main__':
    # 创建标定器实例
    calibrator = CameraCalibrator(square_size=0.035, pattern_size=(9, 7))

    # 标定图像所在目录
    image_dir = './calibration_images'
    os.makedirs(image_dir, exist_ok=True)

    # 提示用户准备图像
    print(f'请将标定图像放入目录: {image_dir}')
    input('准备好后按Enter键继续...')

    # 进行标定
    if calibrator.calibrate(image_dir):
        # 保存标定结果
        calibrator.save_calibration_results('camera_calibration.json')