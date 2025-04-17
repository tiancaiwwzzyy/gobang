import torch
import torch.nn as nn
import torch.optim as optim
import random
import numpy as np
from collections import deque

# 游戏设置
RANGE = 15  # 棋盘大小
EMPTY = 0  # 空子
BLACK = 1  # 黑子
WHITE = 2  # 白子

# DQN 网络定义
class DQN(nn.Module):
    def __init__(self, input_dim, output_dim):
        super(DQN, self).__init__()
        self.fc1 = nn.Linear(input_dim, 128)
        self.fc2 = nn.Linear(128, 128)
        self.fc3 = nn.Linear(128, output_dim)

    def forward(self, x):
        x = torch.relu(self.fc1(x))
        x = torch.relu(self.fc2(x))
        return self.fc3(x)

# 五子棋环境
class GobangEnv:
    def __init__(self):
        self.board = np.zeros((RANGE, RANGE), dtype=int)  # 初始化棋盘
        self.round = 1  # 第一个回合
        self.done = False  # 游戏是否结束

    def reset(self):
        self.board.fill(0)  # 清空棋盘
        self.round = 1
        self.done = False
        return self.board.flatten()

    def display_board(self):
        for row in self.board:
            print(" ".join(str(cell) for cell in row))
        print()

    def step(self, action, player):
        x, y = divmod(action, RANGE)
        if self.board[x, y] != EMPTY:  # 如果该位置已经有棋子，非法操作
            return self.board.flatten(), -10, True  # 惩罚
        self.board[x, y] = player  # 放置棋子
        reward = self.get_reward(x, y, player)
        self.round += 1
        if reward == 1:  # 胜利
            self.done = True
        elif self.round >= RANGE * RANGE:  # 如果棋盘已满，平局
            self.done = True
            reward = 0
        return self.board.flatten(), reward, self.done

    def get_reward(self, x, y, player):
        if self.check_win(x, y, player):
            return 1  # 胜利
        return 0  # 未胜利

    def check_win(self, x, y, player):
        # 判断横、竖、斜是否形成五子连珠
        directions = [(1, 0), (0, 1), (1, 1), (1, -1)]
        for dx, dy in directions:
            count = 1
            for step in range(1, 5):
                nx, ny = x + dx * step, y + dy * step
                if 0 <= nx < RANGE and 0 <= ny < RANGE and self.board[nx, ny] == player:
                    count += 1
                else:
                    break
            for step in range(1, 5):
                nx, ny = x - dx * step, y - dy * step
                if 0 <= nx < RANGE and 0 <= ny < RANGE and self.board[nx, ny] == player:
                    count += 1
                else:
                    break
            if count >= 5:
                return True
        return False
'''
    def check_forbidden(self, x, y, player):
        # 检查三三禁手规则
        # 禁止形成两组活三
        return False  # 暂时返回 False, 禁手规则需要自己实现
'''
# DQN代理
class DQNAgent:
    def __init__(self, input_dim, output_dim):
        self.model = DQN(input_dim, output_dim).float()
        self.target_model = DQN(input_dim, output_dim).float()
        self.target_model.load_state_dict(self.model.state_dict())
        self.optimizer = optim.Adam(self.model.parameters(), lr=0.001)
        self.memory = deque(maxlen=10000)  # 经验回放池
        self.batch_size = 64
        self.gamma = 0.99  # 折扣因子
        self.epsilon = 0.1  # 贪婪策略的 epsilon

    def act(self, state):
        if random.random() < self.epsilon:  # 随机选择动作
            return random.randint(0, RANGE * RANGE - 1)
        state = torch.tensor(state, dtype=torch.float32)
        q_values = self.model(state)
        return torch.argmax(q_values).item()

    def remember(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))

    def replay(self):
        if len(self.memory) < self.batch_size:
            return
        batch = random.sample(self.memory, self.batch_size)
        states, actions, rewards, next_states, dones = zip(*batch)
        
        states = torch.tensor(states, dtype=torch.float32)
        actions = torch.tensor(actions)
        rewards = torch.tensor(rewards)
        next_states = torch.tensor(next_states, dtype=torch.float32)
        dones = torch.tensor(dones)

        q_values = self.model(states)
        next_q_values = self.target_model(next_states)

        target_q_values = q_values.clone()
        for i in range(self.batch_size):
            if dones[i]:
                target_q_values[i, actions[i]] = rewards[i]
            else:
                target_q_values[i, actions[i]] = rewards[i] + self.gamma * torch.max(next_q_values[i])

        loss = nn.MSELoss()(q_values.gather(1, actions.unsqueeze(1)), target_q_values)
        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()

    def update_target_network(self):
        self.target_model.load_state_dict(self.model.state_dict())

# 训练过程
def train():
    # 初始化环境和代理
    env = GobangEnv()
    agent = DQNAgent(input_dim=RANGE * RANGE, output_dim=RANGE * RANGE)

    # 训练过程
    for episode in range(1000000):
        state = env.reset()
        done = False
        while not done:
            action = agent.act(state)
            next_state, reward, done = env.step(action, BLACK)  # 黑方下棋
            agent.remember(state, action, reward, next_state, done)
            agent.replay()
            state = next_state
        
        agent.update_target_network()

        if episode % 100 == 0:
            print(f"Episode {episode}, training done.")

# 导出模型为 ONNX 格式
def export_model():
    agent = DQNAgent(input_dim=RANGE * RANGE, output_dim=RANGE * RANGE)
    torch.onnx.export(agent.model, torch.randn(1, RANGE * RANGE), "gobang_model.onnx")

# 启动训练
train()

# 导出模型
export_model()
