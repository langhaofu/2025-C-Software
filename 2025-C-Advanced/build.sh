#!/bin/bash

# 编译程序脚本
# 使用方法: chmod +x build.sh && ./build.sh

# 设置编译器
CXX=g++

# 设置编译选项
CXXFLAGS="-std=c++11 -Wall -Wextra -O2"

# 设置头文件搜索路径
INCLUDES="-I./include -I/usr/include/opencv4"

# 设置库文件搜索路径和链接选项
LIBS="-lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -pthread"

# 查找源文件
SRC_FILES="src/*.cpp"

# 输出可执行文件名称
OUTPUT="square_detection"

# 创建构建目录
mkdir -p build
cd build

# 编译命令
$CXX $CXXFLAGS $INCLUDES $SRC_FILES -o $OUTPUT $LIBS

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "编译成功! 可执行文件: build/$OUTPUT"
else
    echo "编译失败!"
    exit 1
fi