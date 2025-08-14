#!/bin/bash

# 运行程序脚本
# 使用方法: chmod +x run.sh && ./run.sh

# 检查是否存在编译后的可执行文件
if [ ! -f ./build/square_detection ]; then
    echo "未找到可执行文件，请先运行 ./build.sh 编译程序"
    exit 1
fi

# 运行程序，自动识别 /dev/video0
./build/square_detection

# 检查程序是否正常退出
if [ $? -eq 0 ]; then
    echo "程序正常退出"
else
    echo "程序异常退出，退出码: $?"
fi