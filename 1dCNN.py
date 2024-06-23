'''1维CNN'''
'''2024.06.02'''

# 读取 .mat 文件




import scipy.io
import numpy as np
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset, random_split
from sklearn.preprocessing import LabelEncoder
from sklearn.metrics import classification_report
def load_mat_file(filepath):
    mat = scipy.io.loadmat(filepath)
    # 假设文件中有一个名为 'data' 的变量，这里需要根据实�
文件中的变量名调整
    data = mat['data']
    return data

# 加载所有动作文件的数据并生成标签


def load_all_data(files):
    data_list = []
    labels_list = []
    for idx, file in enumerate(files):
        data = load_mat_file(file)
        # 根据实�
数据形状调整这里的处理方式
        data_list.append(data)
        labels_list.append(np.full(data.shape[0], idx))

    data = np.concatenate(data_list, axis=0)
    labels = np.concatenate(labels_list, axis=0)
    return data, labels


# 示例文件列表（请替换为实�
文件路径）
files = [
    'E:/桌面/竞赛、研学、评奖评优/2024生医工国赛/data/up_tailor_raw.mat',
    'E:/桌面/竞赛、研学、评奖评优/2024生医工国赛/data/down_tailor_raw.mat',
    'E:/桌面/竞赛、研学、评奖评优/2024生医工国赛/data/left_tailor_raw.mat',
    'E:/桌面/竞赛、研学、评奖评优/2024生医工国赛/data/right_tailor_raw.mat',
    'E:/桌面/竞赛、研学、评奖评优/2024生医工国赛/data/wink_tailor_raw.mat'
]

# 加载数据
data, labels = load_all_data(files)
# %%
# 数据形状调整为 (num_samples, signal_length)
signal_length = data.shape[1]

# 标签编码
label_encoder = LabelEncoder()
labels = label_encoder.fit_transform(labels)

# 转换为PyTorch张量
data_tensor = torch.tensor(data, dtype=torch.float32)
labels_tensor = torch.tensor(labels, dtype=torch.int64)

# 创建TensorDataset
dataset = TensorDataset(data_tensor, labels_tensor)

# 拆分训练集和测试集
train_size = int(0.8 * len(dataset))
test_size = len(dataset) - train_size
train_dataset, test_dataset = random_split(dataset, [train_size, test_size])

# 创建DataLoader
batch_size = 32
train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=batch_size, shuffle=False)

# 定义1D CNN模型


class CNN1D(nn.Module):
    def __init__(self, num_classes):
        super(CNN1D, self).__init__()
        self.conv1 = nn.Conv1d(in_channels=1, out_channels=64, kernel_size=3)
        self.pool = nn.MaxPool1d(kernel_size=2)
        self.conv2 = nn.Conv1d(in_channels=64, out_channels=128, kernel_size=3)
        # 计算�
�式：((signal_length - kernel_size + 1) // pool_size)
        self.fc1 = nn.Linear(128 * ((signal_length - 4) // 4), 128)
        self.fc2 = nn.Linear(128, num_classes)

    def forward(self, x):
        x = self.pool(torch.relu(self.conv1(x)))
        x = self.pool(torch.relu(self.conv2(x)))
        x = x.view(-1, 128 * ((signal_length - 4) // 4))
        x = torch.relu(self.fc1(x))
        x = self.fc2(x)
        return x


# 初始化模型、损失函数和优化器
num_classes = len(files)
model = CNN1D(num_classes=num_classes)
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

# 训练模型
num_epochs = 10

for epoch in range(num_epochs):
    model.train()
    running_loss = 0.0
    for inputs, labels in train_loader:
        inputs = inputs.unsqueeze(1)  # 增加通道维度

        optimizer.zero_grad()
        outputs = model(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        running_loss += loss.item() * inputs.size(0)

    epoch_loss = running_loss / train_size
    print(f'Epoch {epoch+1}/{num_epochs}, Loss: {epoch_loss:.4f}')

# 保存模型
model_path = 'eye_signal_cnn_model.pth'
torch.save(model.state_dict(), model_path)
print(f'Model saved to {model_path}')

# 加载模型
model = CNN1D(num_classes=num_classes)
model.load_state_dict(torch.load(model_path))
model.eval()  # 设置为评估模式
print('Model loaded and ready for inference')


def get_real_time_data():
    # 这是一个示例函数，应根据实�
�
况实现
    # 假设返回一个形状为 (signal_length,) 的 numpy 数组
    return np.random.rand(signal_length).astype(np.float32)


# 实时数据分类
real_time_data = get_real_time_data()
real_time_data_tensor = torch.tensor(real_time_data).unsqueeze(
    0).unsqueeze(0)  # 转换为 (1, 1, signal_length) 的张量

with torch.no_grad():
    output = model(real_time_data_tensor)
    _, predicted_label = torch.max(output, 1)

print(f'Predicted label for real-time data: {predicted_label.item()}')
