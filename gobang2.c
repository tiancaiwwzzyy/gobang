// 最优版
//  增加悔棋，复盘，禁手检测功能,排除运行一段时间后停止的问题
#include <stdio.h>
#include <stdlib.h>

#define RANGE 15 // 棋盘大小
#define EMPTY 0  // 空子
#define BLACK 1  // 黑子
#define WHITE 2  // 白子

int dp[RANGE][RANGE]; // 记录落子情况，0为空，1为黑子，2为白子
int round_num = 0;    // 初始化回合数
int x, y;             // 下棋的坐标
int directions[4][2] = {
    {1, 0},
    {0, 1},
    {1, 1},
    {1, -1}};                  // 方向数组
int history[RANGE * RANGE][2]; // 记录历史，每一步的坐标（x, y）

void clear_buffer();                                  // 清空输入缓冲区
void display_board();                                 // 绘制棋盘
void human(int player);                               // 人类方下棋实现函数
int win();                                            // 判断胜负
int end();                                            // 判断结束
void AI(int player);                                  // AI下棋实现函数
int score_position(int x, int y, int player);         // 每一步的评分函数
void best_move(int player, int *best_x, int *best_y); // AI选择最佳下棋位置
void undo_move();                                     // 撤销一步棋
void review();                                        // 复盘
int check_forbidden(int x0, int y0, int player);      // 检查禁手

void display_board()
{
    printf("  "); // 先空几格让数字对齐
    for (int i = 0; i <= RANGE - 1; i++)
    {
        printf("%2d ", i); // 绘制棋盘上方的数字
    }
    printf("\n"); // 换行，正式开始画棋盘

    for (int i = 0; i < RANGE; i++)
    {
        printf("%2d ", i); // 统一格式，让数字对齐
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

int win() // return 1表示存在五子相连，结束游戏
{
    // 判断五子连珠的逻辑与原来保持一致
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
    return 1; // return表示棋盘已满，结束游戏，平局
}

void undo_move()
{
    // 获取上一步的坐标
    round_num -= 1; // 回合数减一，用于确定悔棋后下一步应当由白棋还是黑棋下
    int last_move_x = history[round_num][0];
    int last_move_y = history[round_num][1];
    // 恢复棋盘位置为空
    dp[last_move_x][last_move_y] = EMPTY;
    printf("Undo successful!\n");
    display_board();
}

void review()
{
    printf("Do you want to  review? (1. Yes 2. No): ");
    int choice;
    clear_buffer();
    scanf("%d", &choice);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    if (choice == 1)
    {
        int round = 1;
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                dp[i][j] = 0;
            }
        }
        while (round < round_num)
        {
            int x = history[round][0];
            int y = history[round][1];
            dp[x][y] = round % 2 == 1 ? BLACK : WHITE;
            printf("Round %d: Player %d move: %d %d\n", round, round % 2 == 1 ? BLACK : WHITE, x, y);
            display_board();
            printf("Press Enter to continue...\n");
            round++;
            while (getchar() != '\n')
                ;
        }
        printf("Game over.\n");
    }
    else
    {
        printf("Game over.\n");
        exit(0);
    }
}

void clear_buffer() // 清空输入缓冲区
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int check_forbidden(int x0, int y0, int player) // 实际上与score_position函数中的逻辑基本一致，用于检测三三禁手
{
    int count;
    int forward_space, backward_space;
    int total_count = 0;
    if (player == WHITE)
    {
        return 1;
    }
    else if (player == BLACK)
    {
        for (int i = 0; i < 4; i++) // 遍历四个方向
        {
            count = 1; // 当前棋子算上
            forward_space = 0;
            backward_space = 0;
            // 正向搜索
            for (int j = 1; j < 5; j++)
            {
                int nx = x0 + j * directions[i][0];
                int ny = y0 + j * directions[i][1];
                if (nx >= 0 && nx < RANGE && ny >= 0 && ny < RANGE)
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
                if (nx >= 0 && nx < RANGE && ny >= 0 && ny < RANGE)
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
            // 判断是否有禁手
            if (count == 3 && forward_space == 1 && backward_space == 1)
            {
                total_count++;
            }
        }
        return total_count <= 1;
    }
    return 1;
}

void human(int player)
{
    while (1)
    {
        printf("Player %d, please enter your move (row col, eg:8 8), or enter \'15 15\' to undo your move: ", player);
        clear_buffer();
        if (scanf("%d %d", &x, &y) == 2) // 读取字符，并判断是否为两个数字
        {
            if (x >= 0 && x <= RANGE - 1 && y >= 0 && y <= RANGE - 1 && dp[x][y] == 0 && check_forbidden(x, y, player)) // 判断输入是否合法
            {
                dp[x][y] = player;
                // 记录这一步的坐标
                history[round_num][0] = x;
                history[round_num][1] = y;
                break;
            }
            else if (x == 15 && y == 15) // 输入“15 15”时，悔棋
            {
                if (round_num > 1)
                {
                    undo_move();
                    int choice;
                    choice = 1;
                    while (choice == 1 && round_num >= 1) // 悔棋后，用于判断是否想要连续悔棋
                    {
                        printf("Do you want to undo your move again? (1. Yes ;Press 2): ");
                        clear_buffer();
                        scanf("%d", &choice);
                        if (choice == 1)
                        {
                            if (round_num == 1) // 如果回到开始状态，不能再悔棋
                            {
                                printf("No more moves to undo.\n");
                                break;
                            }
                            undo_move();
                        }
                    }
                    round_num -= 1;
                    break;
                }
                else // 开始状态无法悔棋
                {
                    printf("No more moves to undo.\n");
                    round_num -= 1;
                    break;
                }
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
                    break; // 遇见空位就跳出循环
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
                    break; // 遇见空位就跳出循环
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

        int open_ends = forward_space + backward_space; // 计算开放的端点

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
            if (dp[i][j] == EMPTY && check_forbidden(i, j, player)) // 检测什么时候可以下（包括非禁手和空位），对每一个空位进行评分
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
        // 记录这一步的坐标
        history[round_num][0] = x;
        history[round_num][1] = y;
        printf("AI move: 7 7\n");
    }
    else
    {
        best_move(player, &x, &y);
        dp[x][y] = player;
        // 记录这一步的坐标
        history[round_num][0] = x;
        history[round_num][1] = y;
        printf("AI move: %d %d\n", x, y);
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
    int mod; // 选择模式
    int n;   // 选择黑棋玩家
    printf("Choose a mode: 1. Human vs Human 2. Human vs AI\n");
    scanf("%d", &mod);
    if (mod == 1) // 人人对战
    {
        while (1)
        {
            if (round_num % 2 == 1)
            {
                human(BLACK); // 黑棋先手
                round_num += 1;
                display_board();
                if (win())
                {
                    review();
                    break;
                }
            }
            else if (round_num % 2 == 0)
            {
                human(WHITE);
                round_num += 1;
                display_board();
                if (win())
                {
                    review();
                    break;
                }
            }
            if (end()) // 判断是否平局
            {
                printf("It's a draw!\n");
                review();
                break;
            }
        }
        return 0;
    }
    else if (mod == 2) // 人机对战
    {
        printf("Choose the black player: 1. Human 2. AI\n");
        scanf("%d", &n);
        while (1)
        {
            if (n == 1) // 人类先手
            {
                if (round_num % 2 == 1)
                {
                    human(BLACK);
                    round_num += 1;
                    display_board();
                    if (win())
                    {
                        review();
                        break;
                    }
                }
                else if (round_num % 2 == 0)
                {
                    AI(WHITE);
                    round_num += 1;
                    display_board();
                    if (win())
                    {
                        review();
                        break;
                    }
                }
            }
            else if (n == 2) // AI先手
            {
                if (round_num % 2 == 1)
                {
                    AI(BLACK);
                    round_num += 1;
                    display_board();
                    if (win())
                    {
                        review();
                        break;
                    }
                }
                else if (round_num % 2 == 0)
                {
                    human(WHITE);
                    round_num += 1;
                    display_board();
                    if (win())
                    {
                        review();
                        break;
                    }
                }
            }
            if (end())
            {
                printf("It's a draw!\n");
                review();
                break;
            }
        }
        return 0;
    }
}