#include "Gomoku.h"
void AiGo()
{
    int kill();
    printf("电脑《%s》下棋中……\n", aipiece == BLACK ? "黑方" : "白方");
    findbestmove(&row, &col);
    arrayForInnerBoardLayout[row][col] = aipiece;
    // if (kill())
    //     arrayForInnerBoardLayout[row][col] = aipiece;

    return;
}

int firstmove()
{
    if (aipiece == BLACK && arrayForInnerBoardLayout[SIZE / 2][SIZE / 2] == EMPTY)
    {
        row = SIZE / 2, col = SIZE / 2;
        return 1;
    }
    else if (aipiece == WHITE)
    {
        row = row + ((row < SIZE / 2) ? 1 : -1);
        col = col + ((col < SIZE / 2) ? 1 : -1);
        return 1;
    }
    return 0;
}

int kill()
{
    int i1, j1, i2, j2, count = 0;
    int dir;
    int currentpiece = aipiece;
    int killing = 0;
    int twotimes = 2;
    int killx, killy;
    // 遍历每个方向
    while (twotimes--)
    {
        for (row = 0; row < SIZE; row++)
            for (col = 0; col < SIZE; col++)
                if (arrayForInnerBoardLayout[row][col] == EMPTY && !isalone(row, col))
                {
                    arrayForInnerBoardLayout[row][col] = currentpiece;
                    for (dir = 0; dir < 4; dir++)
                    {
                        count = 0;
                        // 向一个方向移动并计数
                        for (i1 = row, j1 = col; iscurpiece(i1, j1, currentpiece);)
                        {
                            i1 += direction[dir][0];
                            j1 += direction[dir][1];
                            count++;
                        }
                        // 向相反方向移动并计数
                        for (i2 = row, j2 = col; iscurpiece(i2, j2, currentpiece);)
                        {
                            i2 -= direction[dir][0];
                            j2 -= direction[dir][1];
                            count++;
                        }
                        count--; // 初始位置被计了两次
                        // 如果计数为5，则返回获胜标志

                        if (count == 5)
                        {
                            arrayForInnerBoardLayout[row][col] = EMPTY;
                            return 1;
                        }
                        // if (count == 4 && isempty(i1, j1) && isempty(i2, j2) && !killing)
                        // {
                        //     killing = 1;
                        //     killx = row;
                        //     killy = col;
                        // }
                    }
                    arrayForInnerBoardLayout[row][col] = EMPTY;
                }
        currentpiece = manpiece;
    }
    if (killing)
    {
        row = killx;
        col = killy;
        return 1;
    }
    return 0;
}
// 寻找最佳走法，并返回到bestX, bestY中
void findbestmove(int *bestX, int *bestY)
{
    int i, j;
    static int start = 1;
    long long score, maxscore = -INFINITY, beta = INFINITY, alpha = -INFINITY;
    int maxdepth = 3;    // 定义最大搜索深度
    double weight = 1.1; // 权重系数，用于平衡不同层次的分数
    // 开局定式
    if (start)
    {
        firstmove();
        start = 0;
        return;
    }
    // 先算杀
    if (kill())
    {
        arrayForInnerBoardLayout[row][col] = aipiece;
        return;
    }

    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            if (arrayForInnerBoardLayout[i][j] == EMPTY && !isalone(i, j))
            {
                arrayForInnerBoardLayout[i][j] = aipiece;
                // 禁手跳过
                if (aipiece == BLACK && checkForbiddenMoves(i, j))
                {
                    arrayForInnerBoardLayout[i][j] = EMPTY;
                    continue;
                }
                score = minimax(maxdepth, beta, alpha, manpiece, weight * evaluate(i, j));
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

long long int minimax(int depth, long long int beta, long long int alpha, int curpiece, long long int lastscore)
{
    long long score = 0, layerscore = 0;
    int i, j;
    double weight = 1.1; // 权重系数，用于平衡不同层次的分数
    if (depth == 0)
        return lastscore / weight;
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
                    layerscore = evaluate(i, j) + lastscore;
                    score = minimax(depth - 1, beta, alpha, manpiece, weight * layerscore);
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
                    layerscore = evaluate(i, j) + lastscore;
                    score = minimax(depth - 1, beta, alpha, aipiece, weight * layerscore);
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

// 评估函数，返回单个点的评估分数
long long int evaluate(int i, int j)
{
    long long posiScore = 0, negaScore = 0;
    long long int scoretab[11] = {15000000000000, 1500000000, 5000000, 1000000, 100000, 10000, 1000, 2, 1, 10, -15000000000000};
    // 计算该点分数
    int i1, j1, i2, j2, i11, j11, i22, j22; // 分别记录同一条线上的正向与负向
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

            // 禁手
            if (curpiece == BLACK && checkForbiddenMoves(i, j))
                *pscore += scoretab[DEAD];
            // 形成五连直接获胜；
            if (count >= 5)
            {
                *pscore += scoretab[FIVEROW];
            }

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
            i11 = i1 + direction[dir][0];
            j11 = j1 + direction[dir][1];
            i22 = i2 - direction[dir][0];
            j22 = j2 - direction[dir][1]; // 用于继续判断下一格

            if (count == 3)
            {
                if (isempty(i1, j1) && isempty(i2, j2))
                {
                    if (!iscurpiece(i11, j11, curpiece) && !iscurpiece(i22, j22, curpiece))
                    {
                        if (isempty(i11, j11) || isempty(i22, j22))
                            *pscore += scoretab[OPENTHREE];
                        else
                            *pscore += scoretab[HALFOPENTHREE];
                    }
                    else
                    {
                        if (iscurpiece(i11, j11, curpiece))
                            *pscore += scoretab[HALFOPENFOUR];
                        if (iscurpiece(i22, j22, curpiece))
                            *pscore += scoretab[HALFOPENFOUR];
                    }
                }
                else if (isempty(i1, j1))
                {
                    if (iscurpiece(i11, j11, curpiece))
                        *pscore += scoretab[HALFOPENFOUR];
                    else if (isempty(i11, j11))
                        *pscore += scoretab[HALFOPENTHREE];
                    else
                        *pscore += scoretab[SLEEP];
                }
                else if (isempty(i2, j2))
                {
                    if (iscurpiece(i22, j22, curpiece))
                        *pscore += scoretab[HALFOPENFOUR];
                    else if (isempty(i22, j22))
                        *pscore += scoretab[HALFOPENTHREE];
                    else
                        *pscore += scoretab[SLEEP];
                }
                else
                    *pscore += scoretab[SLEEP];
            }

            if (count == 2)
            {
                if (isempty(i1, j1) && isempty(i2, j2))
                {
                    if (iscurpiece(i11, j11, curpiece))
                    {
                        if (isempty(i11 + direction[dir][0], j11 + direction[dir][1]))
                        {
                            *pscore += scoretab[OPENTHREE];
                        }
                        else
                            *pscore += scoretab[HALFOPENTHREE];
                    }
                    if (iscurpiece(i22, j22, curpiece))
                    {
                        if (isempty(i22 - direction[dir][0], j22 - direction[dir][1]))
                        {
                            *pscore += scoretab[OPENTHREE];
                        }
                        else
                            *pscore += scoretab[HALFOPENTHREE];
                    }
                    if (!iscurpiece(i11, j11, curpiece) && !iscurpiece(i22, j22, curpiece))
                        *pscore += scoretab[OPENTWO];
                }
                else if (isempty(i1, j1))
                {
                    if (iscurpiece(i11, j11, curpiece))
                    {
                        if (isempty(i11 + direction[dir][0], j11 + direction[dir][1]))
                        {
                            *pscore += scoretab[HALFOPENTHREE];
                        }
                        else
                            *pscore += scoretab[SLEEP];
                    }
                    else
                        *pscore += scoretab[HALFOPENTWO];
                }
                else if (isempty(i2, j2))
                {
                    if (iscurpiece(i22, j22, curpiece))
                    {
                        if (isempty(i22 - direction[dir][0], j22 - direction[dir][1]))
                        {
                            *pscore += scoretab[HALFOPENTHREE];
                        }
                        else
                            *pscore += scoretab[SLEEP];
                    }
                    else
                        *pscore += scoretab[HALFOPENTWO];
                }
                else
                    *pscore += scoretab[SLEEP];
            }

            if (count == 1)
            {
                if (isempty(i1, j1) && isempty(i2, j2))
                {
                    if (!iscurpiece(i11, j11, curpiece) && !iscurpiece(i22, j22, curpiece))
                        *pscore += scoretab[OPENONE];
                    for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                        piecerow++;
                    if (piecerow == 2)
                    {
                        if (isempty(i11, j11))
                            *pscore += scoretab[OPENTHREE];
                        else
                            *pscore += scoretab[HALFOPENTHREE];
                    }
                    else if (piecerow == 3)
                        *pscore += scoretab[HALFOPENFOUR];
                    for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                        piecerow++;
                    if (piecerow == 2)
                    {
                        if (isempty(i22, j22))
                            *pscore += scoretab[OPENTHREE];
                        else
                            *pscore += scoretab[HALFOPENTHREE];
                    }
                    else if (piecerow == 3)
                        *pscore += scoretab[HALFOPENFOUR];
                }
                else if (isempty(i1, j1))
                {
                    if (!iscurpiece(i11, j11, curpiece))
                        *pscore += scoretab[HALFOPENONE];
                    for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                        piecerow++;
                    if (piecerow == 2)
                    {
                        if (isempty(i11, j11))
                            *pscore += scoretab[HALFOPENTHREE];
                        else
                            *pscore += scoretab[SLEEP];
                    }
                    else if (piecerow == 3)
                        *pscore += scoretab[HALFOPENFOUR];
                }
                else if (isempty(i2, j2))
                {
                    if (!iscurpiece(i22, j22, curpiece))
                        *pscore += scoretab[HALFOPENONE];
                    for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                        piecerow++;
                    if (piecerow == 2)
                    {
                        if (isempty(i22, j22))
                            *pscore += scoretab[HALFOPENTHREE];
                        else
                            *pscore += scoretab[SLEEP];
                    }
                    else if (piecerow == 3)
                        *pscore += scoretab[HALFOPENFOUR];
                }
                else
                    *pscore += scoretab[SLEEP];
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
    }
    return 1;
}