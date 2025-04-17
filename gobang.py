def Win():
    for i in range(0, 15):
        for j in range(0, 15):
            if i <= 10:
                if dp[i][j] == dp[i+1][j] == dp[i+2][j] == dp[i+3][j] == dp[i+4][j] == 1:
                    print("Player 1 wins")
                    return True
                if dp[i][j] == dp[i+1][j] == dp[i+2][j] == dp[i+3][j] == dp[i+4][j] == 2:
                    print("Player 2 wins")
                    return True
            if j <= 10:
                if dp[i][j] == dp[i][j+1] == dp[i][j+2] == dp[i][j+3] == dp[i][j+4] == 1:
                    print("Player 1 wins")
                    return True
                if dp[i][j] == dp[i][j+1] == dp[i][j+2] == dp[i][j+3] == dp[i][j+4] == 2:
                    print("Player 2 wins")
                    return True
            if i <= 10 and j <= 10:
                if dp[i][j] == dp[i+1][j+1] == dp[i+2][j+2] == dp[i+3][j+3] == dp[i+4][j+4] == 1:
                    print("Player 1 wins")
                    return True
                if dp[i][j] == dp[i+1][j+1] == dp[i+2][j+2] == dp[i+3][j+3] == dp[i+4][j+4] == 2:
                    print("Player 2 wins")
                    return True
            if i <= 10 and j >= 4:
                if dp[i][j] == dp[i+1][j-1] == dp[i+2][j-2] == dp[i+3][j-3] == dp[i+4][j-4] == 1:
                    print("Player 1 wins")
                    return True
                if dp[i][j] == dp[i+1][j-1] == dp[i+2][j-2] == dp[i+3][j-3] == dp[i+4][j-4] == 2:
                    print("Player 2 wins")
                    return True

def display_board():
    print("  ", end="")
    for i in range(1, 16):
        print(f"{i:2} ", end="")
    print()
    
    for i in range(15):
        print(f"{i + 1:2} ", end="")
        for j in range(15):
            if dp[i][j] == 0:
                print("·  ", end="")
            elif dp[i][j] == 1:
                print("●  ", end="")
            elif dp[i][j] == 2:
                print("○  ", end="")
        print()

def human(n):
    while True:
        try:
            x, y = map(int, input("输入落子位置: ").split())
            if dp[x - 1][y - 1] == 0:
                dp[x - 1][y - 1] = n
                break
        except:
            print("无效的输入，请重新输入。")
    
def AI(m):
    if round == 1:
        sta_begain()
    else:
        best_x, best_y = best_move(m)
        dp[best_x][best_y] = m

def sta_begain():
    if n == 2:
        dp[7][7] = 1
    else:
        for i in range(0, 15):
            for j in range(0, 15):
                if dp[i][j] == 1:
                    if i < 4 or i > 11 or j < 4 or j > 11:
                        dp[7][7] = 2
                        return
                    else:
                        if i < 7 and j < 7:
                            dp[i + 1][j + 1] = 2
                        elif i < 7 and j > 7:
                            dp[i - 1][j + 1] = 2
                        elif i > 7 and j < 7:
                            dp[i + 1][j - 1] = 2
                        elif i > 7 and j > 7:
                            dp[i - 1][j - 1] = 2
                        elif i == 7 and j < 7:
                            dp[i][j + 1] = 2
                        elif i == 7 and j > 7:
                            dp[i][j - 1] = 2
                        elif i < 7 and j == 7:
                            dp[i + 1][j] = 2
                        elif i > 7 and j == 7:
                            dp[i - 1][j] = 2
                        return

def score_position(x, y, player):
    directions = [(1, 0), (0, 1), (1, 1), (1, -1)]
    score = 0
    for direction in directions:
        count, open_ends = 1, 0

        for step in range(1, 5):
            nx, ny = x + step * direction[0], y + step * direction[1]
            if 0 <= nx < 15 and 0 <= ny < 15 and dp[nx][ny] == player:
                count += 1
            elif 0 <= nx < 15 and 0 <= ny < 15 and dp[nx][ny] == 0:
                open_ends += 1
                break
            else:
                break

        for step in range(1, 5):
            nx, ny = x - step * direction[0], y - step * direction[1]
            if 0 <= nx < 15 and 0 <= ny < 15 and dp[nx][ny] == player:
                count += 1
            elif 0 <= nx < 15 and 0 <= ny < 15 and dp[nx][ny] == 0:
                open_ends += 1
                break
            else:
                break

        if count >= 5:
            score += 10000
        elif count == 4 and open_ends == 2:
            score += 5000
        elif count == 4 and open_ends == 1:
            score += 1000
        elif count == 3 and open_ends == 2:
            score += 500
        elif count == 3 and open_ends == 1:
            score += 100
        elif count == 2 and open_ends == 2:
            score += 50
        else:
            score += count * 10

    return score

def best_move(player):
    max_score = -1
    best_x, best_y = -1, -1

    for i in range(15):
        for j in range(15):
            if dp[i][j] == 0:

                score = score_position(i, j, player) + score_position(i, j, 3 - player)
                if score > max_score:
                    max_score = score
                    best_x, best_y = i, j

    return best_x, best_y

if __name__ == "__main__":
    round = 0
    dp = [[0]*15 for _ in range(15)]
    display_board()
    n = int(input("选择先手方：1. 人类 2. AI\n"))
    if n == 1:
        m = 2
        while True:
            round += 1
            human(n)
            display_board()
            if Win():
                break
            AI(m)
            display_board()
            if Win():
                break
    else:
        m = 1
        while True:
            round += 1
            AI(m)
            display_board()
            if Win():
                break
            human(n)
            display_board()
            if Win():
                break