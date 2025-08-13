# CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(FirmwareProject CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(include)

# 查找源文件
file(GLOB SOURCES "src/*.cpp")

# 添加可执行文件
add_executable(firmware ${SOURCES})

# 可选: 添加编译选项
# target_compile_options(firmware PRIVATE -Wall -Wextra)

# 可选: 链接库
# target_link_libraries(firmware PRIVATE some_library)