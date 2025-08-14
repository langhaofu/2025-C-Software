import torch
import torch.nn as nn
import cv2
import numpy as np
import os

# 定义模型结构（这里假设是一个简单的CNN模型）
class SquareNet(nn.Module):
    def __init__(self, num_classes=10):
        super(SquareNet, self).__init__()
        self.conv1 = nn.Conv2d(3, 32, kernel_size=3, stride=1, padding=1)
        self.relu1 = nn.ReLU()
        self.pool1 = nn.MaxPool2d(kernel_size=2, stride=2)
        self.conv2 = nn.Conv2d(32, 64, kernel_size=3, stride=1, padding=1)
        self.relu2 = nn.ReLU()
        self.pool2 = nn.MaxPool2d(kernel_size=2, stride=2)
        self.conv3 = nn.Conv2d(64, 128, kernel_size=3, stride=1, padding=1)
        self.relu3 = nn.ReLU()
        self.pool3 = nn.MaxPool2d(kernel_size=2, stride=2)
        self.fc1 = nn.Linear(128 * 16 * 16, 1024)
        self.relu4 = nn.ReLU()
        self.fc2 = nn.Linear(1024, num_classes)

    def forward(self, x):
        x = self.pool1(self.relu1(self.conv1(x)))
        x = self.pool2(self.relu2(self.conv2(x)))
        x = self.pool3(self.relu3(self.conv3(x)))
        x = x.view(-1, 128 * 16 * 16)
        x = self.relu4(self.fc1(x))
        x = self.fc2(x)
        return x

# 全局模型变量
model = None
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

# 加载模型
def load_model(model_path):
    global model
    try:
        # 创建模型实例
        model = SquareNet()
        # 加载模型权重
        model.load_state_dict(torch.load(model_path, map_location=device))
        model.to(device)
        model.eval()
        print(f"模型加载成功: {model_path}")
        return True
    except Exception as e:
        print(f"模型加载失败: {e}")
        return False

# 预处理图像
def preprocess_image(image_path):
    try:
        # 读取图像
        image = cv2.imread(image_path)
        if image is None:
            return None
        
        # 调整图像大小
        image = cv2.resize(image, (128, 128))
        
        # 转换为RGB格式
        image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        
        # 归一化
        image = image / 255.0
        
        # 转换为Tensor
        image = torch.from_numpy(image.transpose(2, 0, 1)).float()
        
        # 添加批次维度
        image = image.unsqueeze(0)
        
        return image.to(device)
    except Exception as e:
        print(f"图像预处理失败: {e}")
        return None

# 后处理结果
def postprocess_result(output):
    # 获取预测类别
    _, predicted = torch.max(output.data, 1)
    class_id = predicted.item()
    
    # 返回识别结果
    return f"识别到正方形编号: {class_id}"

# 推理函数
def infer(image_path):
    global model
    if model is None:
        return "错误: 模型未加载"
    
    # 检查图像文件是否存在
    if not os.path.exists(image_path):
        return f"错误: 图像文件不存在: {image_path}"
    
    # 预处理图像
    image_tensor = preprocess_image(image_path)
    if image_tensor is None:
        return "错误: 图像预处理失败"
    
    # 进行推理
    with torch.no_grad():
        output = model(image_tensor)
    
    # 后处理结果
    result = postprocess_result(output)
    
    return result

# 测试代码（如果直接运行此脚本）
if __name__ == '__main__':
    # 加载模型
    model_path = '../../models/best_epoch_weights.pth'
    if load_model(model_path):
        # 测试图像路径
        test_image = 'test_image.jpg'
        result = infer(test_image)
        print(result)