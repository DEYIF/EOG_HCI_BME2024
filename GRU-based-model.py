import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, TensorDataset
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
import pandas as pd

# 加载和准备数据
data = pd.read_csv('FullDataset.csv', header=None, low_memory=False)

X = data.iloc[:-1].values.T  # 转置使样本成为行
y = data.iloc[-1].values     # 最后一行作为标签

# 数据预处理
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42)
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)
y_train -= 1  # 如果标签从1开始，减1使其从0开始
y_test -= 1

# 转换为 Tensor
X_train = torch.tensor(X_train, dtype=torch.float32)
X_test = torch.tensor(X_test, dtype=torch.float32)
y_train = torch.tensor(y_train, dtype=torch.float32)
y_test = torch.tensor(y_test, dtype=torch.float32)

# 数据加载器
train_data = TensorDataset(X_train, y_train)
test_data = TensorDataset(X_test, y_test)
train_loader = DataLoader(train_data, batch_size=64, shuffle=True)
test_loader = DataLoader(test_data, batch_size=64, shuffle=False)

# 模型定义


class GRUModel(nn.Module):
    def __init__(self, input_dim, hidden_dim, layer_dim, output_dim):
        super(GRUModel, self).__init__()
        self.hidden_dim = hidden_dim
        self.layer_dim = layer_dim
        self.gru = nn.GRU(input_dim, hidden_dim, layer_dim, batch_first=True)
        self.fc = nn.Linear(hidden_dim, output_dim)

    def forward(self, x):
        h0 = torch.zeros(self.layer_dim, x.size(0), self.hidden_dim).to(x.device)
        out, _ = self.gru(x, h0)
        out = self.fc(out[:, -1, :])  # 取序列中的最后一个输出
        return out


def calculate_accuracy(model, data_loader):
    model.eval()  # 设置模型为评估模式
    correct = 0
    total = 0
    with torch.no_grad():
        for inputs, labels in data_loader:
            outputs = model(inputs.unsqueeze(-1))  # 确保输入是正确的形状
            _, predicted = torch.max(outputs, 1)  # 选择概率最高的类别
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
    return 100 * correct / total


# 实例化模型，定义损失和优化器
model = GRUModel(input_dim=1, hidden_dim=64, layer_dim=2,
                 output_dim=5)  # output_dim 改为5
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=0.01)

# 训练模型
num_epochs = 20
for epoch in range(num_epochs):
    model.train()  # 设置模型为训练模式
    for inputs, labels in train_loader:
        optimizer.zero_grad()
        outputs = model(inputs.unsqueeze(-1))  # 确保输入是正确的形状
        loss = criterion(outputs, labels.long())  # 确保标签的数据类型适合CrossEntropyLoss
        loss.backward()
        optimizer.step()

    train_acc = calculate_accuracy(model, train_loader)
    test_acc = calculate_accuracy(model, test_loader)
    print(f'Epoch [{epoch+1}/{num_epochs}], Loss: {loss.item():.4f}, Train Acc: {train_acc:.2f}%, Test Acc: {test_acc:.2f}%')
