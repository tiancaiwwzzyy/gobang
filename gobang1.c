// 增加了方便阅读的格式
#include <stdio.h>

#define RANGE 15 // 棋盘大小
#define EMPTY 0  // 空子
#define BLACK 1  // 黑子
#define WHITE 2  // 白子

void display_board();                                 // 绘制棋盘
void human(int player);                               // 人类方下棋实现函数
int Win();                                            // 判断胜负
int end();                                            // 判断结束
void AI(int player);                                  // AI下棋实现函数
int score_position(int x, int y, int player);         // 每一步的评分函数
void best_move(int player, int *best_x, int *best_y); // AI选择最佳下棋位置

int dp[15][15];      // 记录落子情况，0为空，1为黑子，2为白子
float round_num = 0; // 初始化回合数
int x, y;            // 下棋的坐标
int directions[4][2] = {
    {1, 0},
    {0, 1},
    {1, 1},
    {1, -1}}; // 方向数组

void display_board()
{
    printf("  "); // 先空几格让数字对齐
    for (int i = 1; i <= RANGE; i++)
    {
        printf("%2d ", i); // 绘制棋盘上方的数字
    }
    printf("\n"); // 换行，正式开始画棋盘

    for (int i = 0; i < RANGE; i++)
    {
        printf("%2d ", i + 1); // 统一格式，让数字对齐
        for (int j = 0; j < RANGE; j++)
        {
            if (dp[i][j] == EMPTY)
            {
                printf(".  ");
            }
            else if (dp[i][j] == BLACK)
            {
                printf("X  "); // 黑棋
            }
            else if (dp[i][j] == WHITE)
            {
                printf("O  "); // 白棋
            }
        }
        printf("\n");
    }
}

int Win() // return表示存在五子相连，结束游戏
{
    for (int i = 0; i < RANGE; i++)
    {
        for (int j = 0; j < RANGE; j++)
        {
            if (i <= 10) // 判断横向是否有五子相连
            {
                if (dp[i][j] == dp[i + 1][j] && dp[i][j] == dp[i + 2][j] && dp[i][j] == dp[i + 3][j] && dp[i][j] == dp[i + 4][j] && dp[i][j] != 0)
                {
                    if (dp[i][j] == BLACK)
                        printf("Player black wins\n");
                    else
                        printf("Player white wins\n");
                    return 1;
                }
            }
            if (j <= 10) // 判断纵向是否有五子相连
            {
                if (dp[i][j] == dp[i][j + 1] && dp[i][j] == dp[i][j + 2] && dp[i][j] == dp[i][j + 3] && dp[i][j] == dp[i][j + 4] && dp[i][j] != 0)
                {
                    if (dp[i][j] == BLACK)
                        printf("Player black wins\n");
                    else
                        printf("Player white wins\n");
                    return 1;
                }
            }
            if (i <= 10 && j <= 10) // 判断斜向下是否有五子相连
            {
                if (dp[i][j] == dp[i + 1][j + 1] && dp[i][j] == dp[i + 2][j + 2] && dp[i][j] == dp[i + 3][j + 3] && dp[i][j] == dp[i + 4][j + 4] && dp[i][j] != 0)
                {
                    if (dp[i][j] == BLACK)
                        printf("Player black wins\n");
                    else
                        printf("Player white wins\n");
                    return 1;
                }
            }
            if (i <= 10 && j >= 4) // 判断斜向上是否有五子相连
            {
                if (dp[i][j] == dp[i + 1][j - 1] && dp[i][j] == dp[i + 2][j - 2] && dp[i][j] == dp[i + 3][j - 3] && dp[i][j] == dp[i + 4][j - 4] && dp[i][j] != 0)
                {
                    if (dp[i][j] == BLACK)
                        printf("Player black wins\n");
                    else
                        printf("Player white wins\n");
                    return 1;
                }
            }
        }
    }
    return 0;
}

int end()
{
    for (int i = 0; i < RANGE; i++)
    {
        for (int j = 0; j < RANGE; j++)
        {
            if (dp[i][j] == EMPTY) // 检测是否还有空位
            {
                return 0;
            }
        }
    }
    return 1; // return表示棋盘已满，结束游戏
}

void human(int player)
{
    while (1)
    {
        printf("Player %d, please enter your move (row col, eg:8 8): ", player);
        if (scanf("%d %d", &x, &y) == 2) // 读取字符，并判断是否为两个数字
        {
            if (x >= 1 && x <= RANGE && y >= 1 && y <= RANGE && dp[x - 1][y - 1] == 0)
            {
                dp[x - 1][y - 1] = player;
                break;
            }
            else
            {
                printf("Invalid move. Please try again.\n");
            }
        }
        else
        {
            printf("Invalid input. Please enter two numbers.\n");
            while (getchar() != '\n')
                ; // 清空输入缓冲区,防止出现奇怪的问题
        }
    }
}

int score_position(int x0, int y0, int player)
{
    int score = 0;

    for (int i = 0; i < 4; i++)
    {
        int count = 1, forward_space = 0, backward_space = 0;

        // 正向搜索
        for (int j = 1; j < 5; j++)
        {
            int nx = x0 + j * directions[i][0];
            int ny = y0 + j * directions[i][1];
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15)
            {
                if (dp[nx][ny] == player)
                {
                    count++;
                }
                else if (dp[nx][ny] == 0)
                {
                    forward_space++;
                    break;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        // 反向搜索
        for (int j = 1; j < 5; j++)
        {
            int nx = x0 - j * directions[i][0];
            int ny = y0 - j * directions[i][1];
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15)
            {
                if (dp[nx][ny] == player)
                {
                    count++;
                }
                else if (dp[nx][ny] == 0)
                {
                    backward_space++;
                    break;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }

        int open_ends = forward_space + backward_space;

        if (count >= 5)
        {
            score += 10000; // 必胜
        }
        else if (count == 4 && open_ends == 2)
        {
            score += 5000; // 活四
        }
        else if (count == 4 && open_ends == 1)
        {
            score += 1000; // 冲四
        }
        else if (count == 3 && open_ends == 2)
        {
            score += 500; // 活三
        }
        else if (count == 3 && open_ends == 1)
        {
            score += 100; // 冲三
        }
        else if (count == 2 && open_ends == 2)
        {
            score += 50; // 活二
        }
        else
        {
            score += count * 10; // 基础评分
        }
    }
    return score;
}

void best_move(int player, int *best_x, int *best_y)
{
    int max_score = -1; // 最大评分，-1为初始值
    int score;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (dp[i][j] == 0) // 对每一个空位进行评分
            {
                score = score_position(i, j, player) + score_position(i, j, 3 - player); // 自己和对手的评分相加，选择最优解
                if (score > max_score)
                {
                    *best_x = i;
                    *best_y = j;
                    max_score = score;
                }
            }
        }
    }
}

void AI(int player)
{
    if (round_num == 1) // 如果是先手也就是黑棋，第一步下在棋盘中央
    {
        x = 7;
        y = 7;
        dp[x][y] = player;
        printf("AI move: 8 8\n");
    }
    else
    {
        best_move(player, &x, &y);
        dp[x][y] = player;
        printf("AI move: %d %d\n", x + 1, y + 1);
    }
}

int main()
{
    printf("Welcome to my goband game!\n");
    round_num = 1; // 初始化回合数
    for (int i = 0; i < RANGE; i++)
    {
        for (int j = 0; j < RANGE; j++)
        {
            dp[i][j] = 0;
        }
    } // 初始化棋盘存储
    display_board();
    int mod;
    int n;
    printf("Choose a mode: 1. Human vs Human 2. Human vs AI\n");
    scanf("%d", &mod);
    if (mod == 1)
    {
        while (1)
        {
            human(BLACK); // 黑棋先手
            round_num += 0.5;
            display_board();
            if (Win())
                break;
            human(WHITE);
            round_num += 0.5;
            display_board();
            if (Win())
                break;
            if (end()) // 判断是否平局
            {
                printf("It's a draw!\n");
                break;
            }
        }
        return 0;
    }
    if (mod == 2)
    {
        printf("Choose the black player: 1. Human 2. AI\n");
        scanf("%d", &n);
        while (1)
        {
            if (n == 1)
            {
                human(BLACK);
                round_num += 0.5;
                display_board();
                if (Win())
                    break;
                AI(WHITE);
                round_num += 0.5;
            }
            else
            {
                AI(BLACK);
                round_num += 0.5;
                display_board();
                if (Win())
                    break;
                human(WHITE);
                round_num += 0.5;
            }
            display_board();
            if (Win())
                break;
            if (end())
            {
                printf("It's a draw!\n");
                break;
            }
        }
        return 0;
    }
}