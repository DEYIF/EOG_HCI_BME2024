"""
Created on Sun Jun  2 12:10:56 2024

@author: 86184
"""
import scipy.io
import numpy as np
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.model_selection import train_test_split
import torch
from torch.utils.data import TensorDataset, DataLoader
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

# 载入数据


def load_data():
    files = [
        'down_tailor_raw.mat',
        'left_tailor_raw.mat',
        'right_tailor_raw.mat',
        'up_tailor_raw.mat',
        'wink_tailor_raw.mat'
    ]
    actions = ['down', 'left', 'right', 'up', 'wink']
    labels = []

    all_data = []
    for file, action in zip(files, actions):
        action_data = scipy.io.loadmat(file)[action + '_tailor_raw']
        all_data.append(action_data)
        labels.extend([action] * action_data.shape[1])

    # Concatenate all data along the second axis (columns)
    combined_data = np.concatenate(all_data, axis=1)
    return combined_data.T, labels

# 数据预处理


def preprocess(data, labels):
    scaler = StandardScaler()
    data_normalized = scaler.fit_transform(data)
    encoder = LabelEncoder()
    labels_encoded = encoder.fit_transform(labels)
    return data_normalized, labels_encoded

# 数据集划分


def split_data(data, labels):
    X_train, X_test, y_train, y_test = train_test_split(
        data, labels, test_size=0.2, random_state=42)
    X_train, X_val, y_train, y_val = train_test_split(
        X_train, y_train, test_size=0.25, random_state=42)  # 0.25 x 0.8 = 0.2
    return X_train, X_val, X_test, y_train, y_val, y_test

# 定义1D CNN模型


class EyeActionCNN(nn.Module):
    def __init__(self):
        super(EyeActionCNN, self).__init__()
        self.conv1 = nn.Conv1d(1, 32, kernel_size=3, stride=1,
                               padding=1)  # 增加filter数量
        self.conv2 = nn.Conv1d(32, 64, kernel_size=3,
                               stride=1, padding=1)  # 更多的filter
        self.pool = nn.MaxPool1d(kernel_size=2, stride=2, padding=0)
        self.conv3 = nn.Conv1d(64, 128, kernel_size=3, stride=1, padding=1)  # 新增卷积层
        self.dropout = nn.Dropout(0.5)  # Dropout层防止过拟合
        self.fc_input_size = self.calculate_fc_input_size(701)
        self.fc1 = nn.Linear(self.fc_input_size, 256)  # 增加神经元数量
        self.fc2 = nn.Linear(256, 128)  # 新增全连接层
        self.fc3 = nn.Linear(128, 5)

    def calculate_fc_input_size(self, input_length):
        # 经过三次卷积和池化
        output_length = (input_length + 2 * 1 - 3) // 1 + 1
        output_length = (output_length - 2) // 2 + 1
        output_length = (output_length + 2 * 1 - 3) // 1 + 1
        output_length = (output_length - 2) // 2 + 1
        output_length = (output_length + 2 * 1 - 3) // 1 + 1
        output_length = (output_length - 2) // 2 + 1
        return output_length * 128  # 根据最后一个卷积层的filter数量

    def forward(self, x):
        x = x.unsqueeze(1)
        x = F.relu(self.conv1(x))
        x = self.pool(x)
        x = F.relu(self.conv2(x))
        x = self.pool(x)
        x = F.relu(self.conv3(x))
        x = self.pool(x)
        x = self.dropout(x)
        x = x.view(x.size(0), -1)
        x = F.relu(self.fc1(x))
        x = self.dropout(x)
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x


# 主程序
def main():
    data, labels = load_data()
    data_normalized, labels_encoded = preprocess(data, labels)
    X_train, X_val, X_test, y_train, y_val, y_test = split_data(
        data_normalized, labels_encoded)

    # 转换为Tensor
    X_train_t = torch.tensor(X_train, dtype=torch.float32)
    X_val_t = torch.tensor(X_val, dtype=torch.float32)
    X_test_t = torch.tensor(X_test, dtype=torch.float32)
    y_train_t = torch.tensor(y_train, dtype=torch.long)
    y_val_t = torch.tensor(y_val, dtype=torch.long)
    y_test_t = torch.tensor(y_test, dtype=torch.long)

    # 数据加载器
    batch_size = 64
    train_loader = DataLoader(TensorDataset(
        X_train_t, y_train_t), batch_size=batch_size, shuffle=True)
    val_loader = DataLoader(TensorDataset(X_val_t, y_val_t),
                            batch_size=batch_size, shuffle=False)
    test_loader = DataLoader(TensorDataset(X_test_t, y_test_t),
                             batch_size=batch_size, shuffle=False)

    # 模型训练
    model = EyeActionCNN()
    optimizer = optim.Adam(model.parameters(), lr=0.001)
    criterion = nn.CrossEntropyLoss()

    epochs = 10
    for epoch in range(epochs):
        model.train()
        for inputs, labels in train_loader:
            optimizer.zero_grad()
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

        model.eval()
        total = 0
        correct = 0
        with torch.no_grad():
            for inputs, labels in val_loader:
                outputs = model(inputs)
                _, predicted = torch.max(outputs.data, 1)
                total += labels.size(0)
                correct += (predicted == labels).sum().item()

        print(f'Epoch {epoch+1}, Validation Accuracy: {100 * correct / total}%')

    print("Training complete")


if __name__ == "__main__":
    main()
