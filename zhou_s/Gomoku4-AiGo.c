#include "Gomoku.h"
void AiGo()
{
    printf("电脑《%s》下棋中\n", aipiece == BLACK ? "黑方" : "白方");
    findbestmove(&row, &col);
    arrayForInnerBoardLayout[row][col] = aipiece;
    return;
}

// 寻找最佳走法，并返回到bestX, bestY中
void findbestmove(int *bestX, int *bestY)
{
    int i, j;
    long long score, maxscore = -INFINITY, beta = INFINITY, alpha = -INFINITY;
    int maxdepth = 2; // 定义最大搜索深度
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            if (arrayForInnerBoardLayout[i][j] == EMPTY && !isalone(i, j))
            {
                arrayForInnerBoardLayout[i][j] = aipiece;
                score = minimax(maxdepth, beta, alpha, manpiece);
                // arrayForInnerBoardLayout[i][j] = manpiece;
                // score += minimax(maxdepth, beta, alpha, aipiece);
                arrayForInnerBoardLayout[i][j] = EMPTY;
                if (score > maxscore)
                {
                    maxscore = score;
                    alpha = score;
                    *bestX = i;
                    *bestY = j;
                }
            }
        }
    return;
}

long long int minimax(int depth, long long int beta, long long int alpha, int curpiece)
{
    long long score = 0;
    int i, j;
    if (depth == 0)
        return evaluate();
    // max层
    if (curpiece == aipiece)
    {
        long long int maxscore = -INFINITY;
        for (i = 0; i < SIZE; i++)
            for (j = 0; j < SIZE; j++)
            {
                if (arrayForInnerBoardLayout[i][j] == EMPTY && !isalone(i, j))
                {
                    arrayForInnerBoardLayout[i][j] = curpiece;
                    score = minimax(depth - 1, beta, alpha, manpiece);
                    arrayForInnerBoardLayout[i][j] = EMPTY;
                    if (score > maxscore)
                    {
                        maxscore = score;
                        alpha = score;
                    }
                    if (score >= beta)
                        return maxscore;
                }
            }
        return maxscore;
    }
    // min层
    if (curpiece == manpiece)
    {
        long long int minscore = INFINITY;
        for (i = 0; i < SIZE; i++)
            for (j = 0; j < SIZE; j++)
            {
                if (arrayForInnerBoardLayout[i][j] == EMPTY && !isalone(i, j))
                {
                    arrayForInnerBoardLayout[i][j] = curpiece;
                    score = minimax(depth - 1, beta, alpha, aipiece);
                    arrayForInnerBoardLayout[i][j] = EMPTY;
                    if (score < minscore)
                    {
                        minscore = score;
                        beta = score;
                    }
                    if (score <= alpha)
                        return minscore;
                }
            }
        return minscore;
    }
    return 0;
}

// 评估函数，返回整个局面的评估分数
long long int evaluate()
{
    int i, j;
    long long posiScore = 0, negaScore = 0;
    long long int scoretab[11] = {1350000000, 2500000, 52000, 4990, 410, 450, 70, 30, 10, 1, -100000000};
    // 计算每个位置的分数

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            int i1, j1, i2, j2; // 分别记录同一条线上的正向与负向
            int dir;
            int piecerow;
            int count = 0;
            long long int *pscore;
            int curpiece = arrayForInnerBoardLayout[i][j];
            if (arrayForInnerBoardLayout[i][j] != EMPTY)
            {
                if (curpiece == aipiece)
                    pscore = &posiScore;
                else
                    pscore = &negaScore;
                // 遍历每个方向
                for (dir = 0; dir < 4; dir++)
                {
                    count = 0;
                    // 向一个方向移动并计数
                    for (i1 = i, j1 = j; iscurpiece(i1, j1, curpiece);)
                    {
                        i1 += direction[dir][0];
                        j1 += direction[dir][1];
                        count++;
                    }
                    // 向相反方向移动并计数
                    for (i2 = i, j2 = j; iscurpiece(i2, j2, curpiece);)
                    {
                        i2 -= direction[dir][0];
                        j2 -= direction[dir][1];
                        count++;
                    }
                    count--; // 初始位置被计了两次

                    // 注意最终i,j在两个方向上的断子处，所以此处可能是敌方子，空白和边界

                    // 形成五连直接获胜；
                    if (count >= 5)
                    {
                        *pscore += scoretab[FIVEROW];
                    }

                    // 判断禁手

                    // 判断活四和冲四
                    if (count == 4)
                    {
                        if (isempty(i1, j1) && isempty(i2, j2))
                            *pscore += scoretab[OPENFOUR];
                        else if (isempty(i1, j1) || isempty(i2, j2))
                            *pscore += scoretab[HALFOPENFOUR];
                        else
                            *pscore += scoretab[SLEEP];
                    }

                    if (count == 3)
                    {
                        if (isempty(i1, j1) && isempty(i2, j2))
                            *pscore += scoretab[OPENTHREE];
                        else if (isempty(i1, j1) || isempty(i2, j2))
                            *pscore += scoretab[HALFOPENTHREE];
                        else
                            *pscore += scoretab[SLEEP];
                    }

                    if (count == 2)
                    {
                        if (isempty(i1, j1) && isempty(i2, j2))
                            *pscore += scoretab[OPENTWO];
                        else if (isempty(i1, j1) || isempty(i2, j2))
                            *pscore += scoretab[HALFOPENTWO];
                        else
                            *pscore += scoretab[SLEEP];
                    }

                    if (count == 1)
                    {
                        if (isempty(i1, j1) && isempty(i2, j2))
                            *pscore += scoretab[OPENONE];
                        else if (isempty(i1, j1) || isempty(i2, j2))
                            *pscore += scoretab[HALFOPENONE];
                        else
                            *pscore += scoretab[SLEEP];
                    }
                }
            }
        }
    return posiScore - negaScore;
}

int isalone(int x, int y)
{
    int dir, i1, j1, i2, j2;
    for (dir = 0; dir < 4; dir++)
    {
        i1 = x + direction[dir][0];
        j1 = y + direction[dir][1];
        i2 = x - direction[dir][0];
        j2 = y - direction[dir][1];
        if (INBOARD(i1) && INBOARD(j1))
            if (!isempty(i1, j1))
                return 0;
        if (INBOARD(i2) && INBOARD(j2))
            if (!isempty(i2, j2))
                return 0;
        i1 = x + 2 * direction[dir][0];
        j1 = y + 2 * direction[dir][1];
        i2 = x - 2 * direction[dir][0];
        j2 = y - 2 * direction[dir][1];
        if (INBOARD(i1) && INBOARD(j1))
            if (!isempty(i1, j1))
                return 0;
        if (INBOARD(i2) && INBOARD(j2))
            if (!isempty(i2, j2))
                return 0;
        return 1;
    }
}