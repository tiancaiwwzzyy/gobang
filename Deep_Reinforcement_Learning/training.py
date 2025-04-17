import random
import numpy as np
from DQN import create_dqn_model
from reinforcement import GomokuEnv

class DQNAgent:
    def __init__(self, model, env, epsilon=1.0, epsilon_min=0.1, epsilon_decay=0.995, gamma=0.99):
        self.model = model
        self.env = env
        self.epsilon = epsilon
        self.epsilon_min = epsilon_min
        self.epsilon_decay = epsilon_decay
        self.gamma = gamma
        self.memory = []

    def remember(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))
    
    def act(self, state):
        if np.random.rand() <= self.epsilon:
            # 随机选择一个动作
            return random.choice(self.env.get_valid_actions())
        else:
            # 选择 Q 值最大的动作
            q_values = self.model.predict(state)
            valid_actions = self.env.get_valid_actions()
            valid_q_values = [q_values[0, action[0] * self.env.board_size + action[1]] for action in valid_actions]
            return valid_actions[np.argmax(valid_q_values)]
    
    def replay(self, batch_size=32):
        if len(self.memory) < batch_size:
            return
        batch = random.sample(self.memory, batch_size)
        
        for state, action, reward, next_state, done in batch:
            target = reward
            if not done:
                next_q_values = self.model.predict(next_state)
                target += self.gamma * np.max(next_q_values)
            
            target_q_values = self.model.predict(state)
            target_q_values[0, action[0] * self.env.board_size + action[1]] = target
            self.model.fit(state, target_q_values, epochs=1, verbose=0)
        
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay

# 初始化环境和模型
env = GomokuEnv()
model = create_dqn_model()
agent = DQNAgent(model, env)

# 开始训练
for episode in range(1000):
    state = env.reset()
    state = np.expand_dims(state, axis=0)  # 添加批次维度
    done = False
    
    while not done:
        action = agent.act(state)
        next_state, reward, done = env.step(action)
        next_state = np.expand_dims(next_state, axis=0)  # 添加批次维度
        agent.remember(state, action, reward, next_state, done)
        agent.replay(batch_size=32)
        state = next_state
    
    if episode % 100 == 0:
        print(f"Episode {episode}: Epsilon = {agent.epsilon}")