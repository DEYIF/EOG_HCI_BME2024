import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import OneHotEncoder
from sklearn.metrics import confusion_matrix
import pandas as pd
import numpy as np

# 加载和准备数据
data = pd.read_csv('FullDataset.csv', header=None, low_memory=False)

X = data.iloc[:-1].values.T  # 转置使样本成为行
y = data.iloc[-1].values     # 最后一行作为标签

# One-Hot编码
encoder = OneHotEncoder()
y_encoded = encoder.fit_transform(y.reshape(-1, 1)).toarray()

# 数据分割
X_train, X_test, y_train, y_test = train_test_split(
    X, y_encoded, test_size=0.2, random_state=42)

# 转换为张量
X_train_tensor = torch.tensor(X_train.astype(np.float32))
X_test_tensor = torch.tensor(X_test.astype(np.float32))
y_train_tensor = torch.tensor(y_train.astype(np.float32))
y_test_tensor = torch.tensor(y_test.astype(np.float32))

# 数据加载器
train_dataset = TensorDataset(X_train_tensor, y_train_tensor)
test_dataset = TensorDataset(X_test_tensor, y_test_tensor)
train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)
test_loader = DataLoader(test_dataset, batch_size=64)

# 定义模型


class CNN1D(nn.Module):
    def __init__(self):
        super(CNN1D, self).__init__()
        self.conv1 = nn.Conv1d(1, 16, kernel_size=5, padding=2)
        self.conv2 = nn.Conv1d(16, 32, kernel_size=5, padding=2)
        self.relu = nn.ReLU()
        self.pool = nn.MaxPool1d(2)
        self.dropout = nn.Dropout(0.5)
        self.fc1 = nn.Linear(32 * (X_train.shape[1] // 4), 100)
        self.fc2 = nn.Linear(100, y_encoded.shape[1])

    def forward(self, x):
        x = x.unsqueeze(1)
        x = self.conv1(x)
        x = self.relu(x)
        x = self.pool(x)
        x = self.conv2(x)
        x = self.relu(x)
        x = self.pool(x)
        x = torch.flatten(x, start_dim=1)
        x = self.dropout(x)
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        return x


model = CNN1D()
criterion = nn.BCEWithLogitsLoss()
optimizer = optim.Adam(model.parameters(), lr=0.001)

# 训练模型并输出准确率


def train_model(num_epochs):
    for epoch in range(num_epochs):
        model.train()
        train_loss = 0
        train_correct = 0
        total_train = 0

        for inputs, labels in train_loader:
            optimizer.zero_grad()
            outputs = model(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()
            train_loss += loss.item()

            predicted = torch.sigmoid(outputs).data > 0.5
            train_correct += (predicted == labels.byte()).all(dim=1).sum().item()
            total_train += labels.size(0)

        train_accuracy = 100 * train_correct / total_train

        # 验证集准确率
        model.eval()
        val_loss = 0
        val_correct = 0
        total_val = 0
        all_preds = []
        all_labels = []
        with torch.no_grad():
            for inputs, labels in test_loader:
                outputs = model(inputs)
                loss = criterion(outputs, labels)
                val_loss += loss.item()

                predicted = torch.sigmoid(outputs).data > 0.5
                val_correct += (predicted == labels.byte()).all(dim=1).sum().item()
                total_val += labels.size(0)
                all_preds.extend(predicted.numpy())
                all_labels.extend(labels.numpy())

        val_accuracy = 100 * val_correct / total_val
        print(f'Epoch {epoch + 1}, Training Loss: {train_loss / len(train_loader)}, Training Accuracy: {train_accuracy}%, Validation Accuracy: {val_accuracy}%')

    # 计算混淆矩阵
    cm = confusion_matrix(np.array(all_labels).argmax(axis=1),
                          np.array(all_preds).argmax(axis=1))
    print(f"Final Validation Loss: {val_loss / len(test_loader)}")
    print(f"Final Validation Accuracy: {val_accuracy}%")
    print("Confusion Matrix:\n", cm)


train_model(50)

# 保存模型
torch.save(model.state_dict(), 'model.pth')
print("模型已保存到 model.pth")
