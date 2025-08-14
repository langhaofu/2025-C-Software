#ifndef PYTHON_WRAPPER_H
#define PYTHON_WRAPPER_H

#include <string>

bool initPython();
void cleanupPython();

class PythonWrapper {
public:
    PythonWrapper();
    ~PythonWrapper();

    // 加载模型
    bool loadModel(const std::string& modelPath);

    // 识别正方形
    std::string recognizeSquare(const std::string& imagePath);
};

#endif // PYTHON_WRAPPER_H