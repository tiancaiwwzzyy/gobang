// 用于比较AI强度
#include <stdio.h>
#include <stdlib.h>
#include "Gomoku.h"

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
    {1, -1}}; // 方向数组
int direction[4][2] = {{0, 1}, {1, 0}, {1, 1}, {-1, 1}};
int history[RANGE * RANGE][2]; // 记录历史，每一步的坐标（x, y）
int AI_player;
int HUMAN_player;
int count_open = 0; // 开放和连珠的记录

void clear_buffer();    // 清空输入缓冲区
void display_board();   // 绘制棋盘
void human(int player); // 人类方下棋实现函数
int win();              // 判断胜负
int end();              // 判断结束
void AI1(int player);
void best_move1(int player, int *best_x, int *best_y);
int score_position1(int x, int y, int player); // 每一步的评分函数
void AI(int player);                           // AI下棋实现函数
int score_position1(int x, int y, int player); // 每一步的评分函数
int score_position(int x, int y, int player);  // 每一步的评分函数
void best_move(int player, int *best_x, int *best_y);
void undo_move(); // 撤销一步棋
void review();    // 复盘
int check_forbidden(int x0, int y0, int player);

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

int win() // return表示存在五子相连，结束游戏
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
    // 恢复棋盘
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

int check_forbidden(int x0, int y0, int player)
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
// AI1

int score_position1(int x0, int y0, int player)
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
        count_open = count + open_ends;
    }
    return score;
}

int avoid(int player, int *x1, int *y1, int *x2, int *y2)
{
    int opponent = (player == 1) ? 2 : 1; // 1 表示黑棋，2 表示白棋

    for (int i = 0; i < RANGE; i++)
    {
        for (int j = 0; j < RANGE; j++)
        {
            if (dp[i][j] == opponent)
            {
                for (int d = 0; d < 4; d++)
                {
                    int count = 1;
                    int nx1 = i - directions[d][0];
                    int ny1 = j - directions[d][1];
                    int nx2 = i + 3 * directions[d][0];
                    int ny2 = j + 3 * directions[d][1];

                    if (nx1 >= 0 && nx1 < RANGE && ny1 >= 0 && ny1 < RANGE && dp[nx1][ny1] == EMPTY &&
                        nx2 >= 0 && nx2 < RANGE && ny2 >= 0 && ny2 < RANGE && dp[nx2][ny2] == EMPTY)
                    {
                        for (int k = 1; k < 3; k++)
                        {
                            int nx = i + k * directions[d][0];
                            int ny = j + k * directions[d][1];
                            if (nx >= 0 && nx < RANGE && ny >= 0 && ny < RANGE && dp[nx][ny] == opponent)
                            {
                                count++;
                            }
                            else
                            {
                                break;
                            }
                        }
                        if (count == 3)
                        {
                            *x1 = nx1;
                            *y1 = ny1;
                            *x2 = nx2;
                            *y2 = ny2;
                            return 1; // 找到活三
                        }
                    }
                }
            }
        }
    }
    *x1 = 15;
    *y1 = 15;
    *x2 = 15;
    *y2 = 15;
    return 0; // 没有找到活三
}

void best_move1(int player, int *best_x, int *best_y)
{
    int max_score = -10000; // 最大评分，-1为初始值

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
    score_position(*best_x, *best_y, player); // 为判断是否执行avoid函数，再次调用score_position函数，获得count_open的值
    int x1, y1, x2, y2;
    if (avoid(player, &x1, &y1, &x2, &y2) && count_open < 5)
    {
        if ((score_position(x1, y1, player) + score_position(x1, y1, 3 - player)) > (score_position(x2, y2, player) + score_position(x2, y2, 3 - player)))
        {
            *best_x = x1;
            *best_y = y1;
        }
        else
        {
            *best_x = x2;
            *best_y = y2;
        }
    }
}

void AI1(int player)
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
        best_move1(player, &x, &y);
        dp[x][y] = player;
        // 记录这一步的坐标
        history[round_num][0] = x;
        history[round_num][1] = y;
        printf("AI move: %d %d\n", x, y);
    }
}
// AI2放这
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
            if (dp[i][j] == 0 && check_forbidden(i, j, player)) // 检测什么时候可以下（包括非禁手和空位），对每一个空位进行评分
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

////
int main()
{
    printf("AI compition!\n");
    round_num = 1; // 初始化回合数
    for (int i = 0; i < RANGE; i++)
    {
        for (int j = 0; j < RANGE; j++)
        {
            dp[i][j] = 0;
        }
    } // 初始化棋盘存储
    display_board();
    int n;
    printf("Choose the black player: 1. AI1 2. AI\n");
    scanf("%d", &n);
    while (1)
    {
        if (n == 1)
        {
            AI_player = BLACK;
            HUMAN_player = WHITE;
            if (round_num % 2 == 1)
            {
                AI1(BLACK);
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
        else if (n == 2)
        {
            AI_player = WHITE;
            HUMAN_player = BLACK;
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
                AI1(WHITE);
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
