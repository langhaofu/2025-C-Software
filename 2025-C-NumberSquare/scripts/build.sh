#!/bin/bash

# 编译和运行脚本

# 创建构建目录（如果不存在）
mkdir -p ../build

# 编译C++代码
echo "开始编译..."
g++ -std=c++11 ../src/main.cpp ../src/python_wrapper.cpp -o ../build/number_square_recognizer \
    -I/usr/include/python3.8 \
    -lpython3.8 \
    -I../src

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "编译成功!"
    echo "开始运行程序..."
    # 运行程序
    cd ../build
    ./number_square_recognizer
else
    echo "编译失败!"
    exit 1
fi