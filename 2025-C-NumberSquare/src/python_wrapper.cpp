#include <Python.h>
#include <iostream>
#include <string>
#include "python_wrapper.h"

// 全局变量，用于存储Python模块指针
static PyObject* pModule = nullptr;
static PyObject* pInferFunc = nullptr;
static PyObject* pLoadModelFunc = nullptr;

// 初始化Python解释器
bool initPython() {
    // 初始化Python
    Py_Initialize();

    // 检查Python是否成功初始化
    if (!Py_IsInitialized()) {
        std::cerr << "Python initialization failed" << std::endl;
        return false;
    }

    // 添加当前路径到Python路径
    PyObject* sysPath = PySys_GetObject((char*)"path");
    PyObject* currentPath = PyUnicode_FromString(".");
    PyList_Append(sysPath, currentPath);
    Py_DECREF(currentPath);

    // 导入Python模块
    pModule = PyImport_ImportModule((char*)"src.python.infer");
    if (!pModule) {
        std::cerr << "Failed to import module 'src.python.infer'" << std::endl;
        PyErr_Print();
        return false;
    }

    // 获取加载模型的函数
    pLoadModelFunc = PyObject_GetAttrString(pModule, (char*)"load_model");
    if (!pLoadModelFunc || !PyCallable_Check(pLoadModelFunc)) {
        std::cerr << "Failed to get 'load_model' function" << std::endl;
        PyErr_Print();
        return false;
    }

    // 获取识别函数
    pInferFunc = PyObject_GetAttrString(pModule, (char*)"infer");
    if (!pInferFunc || !PyCallable_Check(pInferFunc)) {
        std::cerr << "Failed to get 'infer' function" << std::endl;
        PyErr_Print();
        return false;
    }

    return true;
}

// 清理Python解释器
void cleanupPython() {
    if (pInferFunc) {
        Py_DECREF(pInferFunc);
        pInferFunc = nullptr;
    }

    if (pLoadModelFunc) {
        Py_DECREF(pLoadModelFunc);
        pLoadModelFunc = nullptr;
    }

    if (pModule) {
        Py_DECREF(pModule);
        pModule = nullptr;
    }

    // 清理Python解释器
    Py_Finalize();
}

// Python包装器类实现
PythonWrapper::PythonWrapper() {
}

PythonWrapper::~PythonWrapper() {
}

// 加载模型
bool PythonWrapper::loadModel(const std::string& modelPath) {
    if (!pLoadModelFunc) {
        std::cerr << "'load_model' function not initialized" << std::endl;
        return false;
    }

    // 准备函数参数
    PyObject* pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(modelPath.c_str()));

    // 调用Python函数
    PyObject* pResult = PyObject_CallObject(pLoadModelFunc, pArgs);
    Py_DECREF(pArgs);

    if (!pResult) {
        std::cerr << "Failed to call 'load_model' function" << std::endl;
        PyErr_Print();
        return false;
    }

    // 解析结果
    bool success = PyBool_Check(pResult) && PyObject_IsTrue(pResult);
    Py_DECREF(pResult);

    return success;
}

// 识别正方形
std::string PythonWrapper::recognizeSquare(const std::string& imagePath) {
    if (!pInferFunc) {
        std::cerr << "'infer' function not initialized" << std::endl;
        return "Error: function not initialized";
    }

    // 准备函数参数
    PyObject* pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(imagePath.c_str()));

    // 调用Python函数
    PyObject* pResult = PyObject_CallObject(pInferFunc, pArgs);
    Py_DECREF(pArgs);

    if (!pResult) {
        std::cerr << "Failed to call 'infer' function" << std::endl;
        PyErr_Print();
        return "Error: function call failed";
    }

    // 解析结果
    std::string result;
    if (PyUnicode_Check(pResult)) {
        const char* cStr = PyUnicode_AsUTF8(pResult);
        if (cStr) {
            result = cStr;
        } else {
            result = "Error: failed to convert result to string";
        }
    } else {
        result = "Error: invalid result type";
    }

    Py_DECREF(pResult);
    return result;
}