// 原始版，算是正常人，有时会输（与人类）
#include <stdio.h>

void display_board();
void human(int player);
int Win();
int end();
void AI(int player);
int score_position(int x, int y, int player);
void best_move(int player, int *best_x, int *best_y);

int dp[15][15];
int round_num = 0;

void display_board()
{
    printf("  ");
    for (int i = 1; i <= 15; i++)
    {
        printf("%2d ", i);
    }
    printf("\n");

    for (int i = 0; i < 15; i++)
    {
        printf("%2d ", i + 1);
        for (int j = 0; j < 15; j++)
        {
            if (dp[i][j] == 0)
            {
                printf(".  ");
            }
            else if (dp[i][j] == 1)
            {
                printf("X  ");
            }
            else if (dp[i][j] == 2)
            {
                printf("O  ");
            }
        }
        printf("\n");
    }
}

int Win()
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (i <= 10)
            {
                if (dp[i][j] == dp[i + 1][j] && dp[i][j] == dp[i + 2][j] && dp[i][j] == dp[i + 3][j] && dp[i][j] == dp[i + 4][j] && dp[i][j] != 0)
                {
                    printf("Player %d wins\n", dp[i][j]);
                    return 1;
                }
            }
            if (j <= 10)
            {
                if (dp[i][j] == dp[i][j + 1] && dp[i][j] == dp[i][j + 2] && dp[i][j] == dp[i][j + 3] && dp[i][j] == dp[i][j + 4] && dp[i][j] != 0)
                {
                    printf("Player %d wins\n", dp[i][j]);
                    return 1;
                }
            }
            if (i <= 10 && j <= 10)
            {
                if (dp[i][j] == dp[i + 1][j + 1] && dp[i][j] == dp[i + 2][j + 2] && dp[i][j] == dp[i + 3][j + 3] && dp[i][j] == dp[i + 4][j + 4] && dp[i][j] != 0)
                {
                    printf("Player %d wins\n", dp[i][j]);
                    return 1;
                }
            }
            if (i <= 10 && j >= 4)
            {
                if (dp[i][j] == dp[i + 1][j - 1] && dp[i][j] == dp[i + 2][j - 2] && dp[i][j] == dp[i + 3][j - 3] && dp[i][j] == dp[i + 4][j - 4] && dp[i][j] != 0)
                {
                    printf("Player %d wins\n", dp[i][j]);
                    return 1;
                }
            }
        }
    }
    return 0;
}

int end()
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (dp[i][j] == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

void human(int player)
{
    int x, y;
    while (1)
    {
        printf("Player %d, please enter your move (row col): ", player);
        if (scanf("%d %d", &x, &y) == 2)
        {
            if (x >= 1 && x <= 15 && y >= 1 && y <= 15 && dp[x - 1][y - 1] == 0)
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
                ;
        }
    }
}

int score_position(int x, int y, int player)
{
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    int score = 0;
    for (int i = 0; i < 4; i++)
    {
        int count = 1, forward_space = 0, backward_space = 0;

        for (int j = 1; j < 5; j++)
        {
            int nx = x + j * directions[i][0];
            int ny = y + j * directions[i][1];
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && dp[nx][ny] == player)
            {
                count++;
            }
            else if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && dp[nx][ny] == 0)
            {
                forward_space++;
                break;
            }
            else
            {
                break;
            }
        }
        for (int j = 1; j < 5; j++)
        {
            int nx = x - j * directions[i][0];
            int ny = y - j * directions[i][1];
            if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && dp[nx][ny] == player)
            {
                count++;
            }
            else if (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && dp[nx][ny] == 0)
            {
                backward_space++;
                break;
            }
            else
            {
                break;
            }
        }
        int open_ends = forward_space + backward_space;
        if (count >= 5)
        {
            score += 10000;
        }
        else if (count == 4 && open_ends == 2)
        {
            score += 5000;
        }
        else if (count == 4 && open_ends == 1)
        {
            score += 1000;
        }
        else if (count == 3 && open_ends == 2)
        {
            score += 500;
        }
        else if (count == 3 && open_ends == 1)
        {
            score += 100;
        }
        else if (count == 2 && open_ends == 2)
        {
            score += 50;
        }
        else
        {
            score += count * 10;
        }
        if (forward_space + backward_space >= 2)
        {
            score += count * 20;
        }
    }
    return score;
}

void best_move(int player, int *best_x, int *best_y)
{
    int max_score = -1;

    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (dp[i][j] == 0)
            {
                int score = score_position(i, j, player) + score_position(i, j, 3 - player);
                if (score > max_score)
                {
                    *best_x = i;
                    *best_y = j;
                }
            }
        }
    }
}

void AI(int player)
{
    int x, y;
    if (round_num == 1)
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
    round_num = 0;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            dp[i][j] = 0;
        }
    }
    display_board();
    int n;
    printf("Choose the first player: 1. Human 2. AI\n");
    scanf("%d", &n);
    while (1)
    {
        round_num++;
        if (n == 1)
        {
            human(1);
            display_board();
            if (Win())
                break;
            AI(2);
        }
        else
        {
            AI(1);
            display_board();
            if (Win())
                break;
            human(2);
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