import numpy as np
import random

# 定义五子棋环境
class GomokuEnv:
    def __init__(self, board_size=15):
        self.board_size = board_size
        self.board = np.zeros((board_size, board_size), dtype=int)  # 0: 空白, 1: 黑棋, -1: 白棋
        self.current_player = 1  # 1: 黑棋, -1: 白棋

    def reset(self):
        self.board = np.zeros((self.board_size, self.board_size), dtype=int)
        self.current_player = 1
        return self.board

    def step(self, action):
        x, y = action
        if self.board[x, y] != 0:
            raise ValueError("Invalid move, cell already occupied.")
        
        # 放置棋子
        self.board[x, y] = self.current_player
        
        # 检查游戏是否结束
        winner = self.check_winner(x, y)
        if winner != 0:
            reward = 1 if winner == 1 else -1  # 黑棋胜利为+1，白棋胜利为-1
            done = True
        else:
            reward = 0  # 游戏继续
            done = False
        
        # 切换玩家
        self.current_player *= -1
        
        return self.board, reward, done

    def check_winner(self, x, y):
        # 检查在(x, y)的位置，是否有五子连珠
        player = self.board[x, y]
        
        # 定义方向（横、纵、斜）
        directions = [(1, 0), (0, 1), (1, 1), (1, -1)]
        
        for dx, dy in directions:
            count = 1
            for i in range(1, 5):
                nx, ny = x + i * dx, y + i * dy
                if 0 <= nx < self.board_size and 0 <= ny < self.board_size and self.board[nx, ny] == player:
                    count += 1
                else:
                    break

            for i in range(1, 5):
                nx, ny = x - i * dx, y - i * dy
                if 0 <= nx < self.board_size and 0 <= ny < self.board_size and self.board[nx, ny] == player:
                    count += 1
                else:
                    break

            if count >= 5:
                return player  # 返回胜利方（1 或 -1）

        return 0  # 无胜利方

    def get_valid_actions(self):
        return [(i, j) for i in range(self.board_size) for j in range(self.board_size) if self.board[i, j] == 0]