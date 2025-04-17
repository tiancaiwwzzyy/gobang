#include "Gomoku.h"

// 方向数组，表示四个方向的移动
int direction[4][2] = {{0, 1}, {1, 0}, {1, 1}, {-1, 1}};

/**
 * 检查当前棋局是否有获胜方
 * @return 返回获胜标志（END）或继续游戏标志（GOON）
 */
int checkwin()
{
    int i, j, count = 0;
    int dir;
    int currentpiece = arrayForInnerBoardLayout[row][col];
    // 遍历每个方向
    for (dir = 0; dir < 4; dir++)
    {
        count = 0;
        // 向一个方向移动并计数
        for (i = row, j = col; iscurpiece(i, j, currentpiece);)
        {
            i += direction[dir][0];
            j += direction[dir][1];
            count++;
        }
        // 向相反方向移动并计数
        for (i = row, j = col; iscurpiece(i, j, currentpiece);)
        {
            i -= direction[dir][0];
            j -= direction[dir][1];
            count++;
        }
        count--; // 初始位置被计了两次
        // 如果计数为5，则返回获胜标志
        if ((currentpiece == BLACK && count == 5) || (currentpiece == WHITE && count >= 5))
            return END;
    }
    // 继续游戏
    return GOON;
}

int checkForbiddenMoves(int x, int y)
{
    int currentpiece = arrayForInnerBoardLayout[row][col]; //=BLACK
    int liveThreeCount = 0;                                // 活三
    int openFourCount = 0;                                 // 活四
    int halfOpenFourCount = 0;                             // 冲四
    int longChainCount = 0;                                // 长连
    int count = 0;                                         // 记录连子总数
    int i1, j1, i2, j2, i11, j11, i22, j22;                // 分别记录同一条线上的正向与负向
    // i1为正向横坐标，i11是i1后面一格
    int dir;
    int piecerow;
    int curpiece = BLACK;

    // 遍历每个方向
    for (dir = 0; dir < 4; dir++)
    {
        count = 0;
        // 向一个方向移动并计数
        for (i1 = x, j1 = y; iscurpiece(i1, j1, curpiece);)
        {
            i1 += direction[dir][0];
            j1 += direction[dir][1];
            count++;
        }
        // 向相反方向移动并计数
        for (i2 = x, j2 = y; iscurpiece(i2, j2, curpiece);)
        {
            i2 -= direction[dir][0];
            j2 -= direction[dir][1];
            count++;
        }
        count--; // 初始位置被计了两次

        // 注意最终i,j在两个方向上的断子处，所以此处可能是敌方子，空白和边界

        // 形成五连直接获胜；
        if (count == 5)
            return GOON;

        // 判断长连
        if (count > 5)
        {
            longChainCount++;
            return END;
        }

        // 判断活四和连续冲四
        if (count == 4)
        {
            if (isempty(i1, j1) && isempty(i2, j2))
                openFourCount++;
            else if (isempty(i1, j1) || isempty(i2, j2))
                halfOpenFourCount++;
        }

        i11 = i1 + direction[dir][0];
        j11 = j1 + direction[dir][1];
        i22 = i2 - direction[dir][0];
        j22 = j2 - direction[dir][1]; // 用于继续判断下一格

        // 判断连续活三和不连续的冲四
        if (count == 3)
        {
            if (isempty(i1, j1) && isempty(i2, j2))
            {
                if (!iscurpiece(i11, j11, curpiece) && !iscurpiece(i22, j22, curpiece))
                {
                    if (isempty(i11, j11) || isempty(i22, j22))
                        liveThreeCount++;
                }
                else
                {
                    if (iscurpiece(i11, j11, curpiece))
                        halfOpenFourCount++;
                    if (iscurpiece(i22, j22, curpiece))
                        halfOpenFourCount++;
                }
            }
            else if (isempty(i1, j1))
            {
                if (iscurpiece(i11, j11, curpiece))
                    halfOpenFourCount++;
            }
            else if (isempty(i2, j2))
            {
                if (iscurpiece(i22, j22, curpiece))
                    halfOpenFourCount++;
            }
        }
        // 判断不连续活三和不连续冲四
        if (count == 2)
        {
            if (isempty(i1, j1) && isempty(i2, j2))
            {
                if (iscurpiece(i11, j11, curpiece) && isempty(i11 + direction[dir][0], j11 + direction[dir][1]))
                    if (!iscurpiece(i11 + 2 * direction[dir][0], j11 + 2 * direction[dir][1], curpiece))
                        liveThreeCount++;
                if (iscurpiece(i22, j22, curpiece) && isempty(i22 - direction[dir][0], j22 - direction[dir][1]))
                    if (!iscurpiece(i22 - 2 * direction[dir][0], j22 - 2 * direction[dir][1], curpiece))
                        liveThreeCount++;
            }
            if (isempty(i1, j1))
            {
                for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                    piecerow++;
                if (piecerow == 2)
                    halfOpenFourCount++;
            }
            if (isempty(i2, j2))
            {
                for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                    piecerow++;
                if (piecerow == 2)
                    halfOpenFourCount++;
            }
        }
        // 判断不连续活三和不连续冲四
        if (count == 1)
        {
            if (isempty(i1, j1) && isempty(i2, j2))
            {
                for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                    piecerow++;
                if (piecerow == 2)
                    if (isempty(i11, j11) && (!iscurpiece(i22, j22, curpiece)))
                        liveThreeCount++;

                i11 = i1 + direction[dir][0];
                j11 = j1 + direction[dir][1];

                for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                    piecerow++;
                if (piecerow == 2)
                    if (isempty(i22, j22) && (!iscurpiece(i11, j11, curpiece)))
                        liveThreeCount++;
            }
            i11 = i1 + direction[dir][0];
            j11 = j1 + direction[dir][1];
            i22 = i2 - direction[dir][0];
            j22 = j2 - direction[dir][1];
            if (isempty(i1, j1))
            {
                for (piecerow = 0; iscurpiece(i11, j11, curpiece); i11 += direction[dir][0], j11 += direction[dir][1])
                    piecerow++;
                if (piecerow == 3)
                    halfOpenFourCount++;
            }
            if (isempty(i2, j2))
            {
                for (piecerow = 0; iscurpiece(i22, j22, curpiece); i22 -= direction[dir][0], j22 -= direction[dir][1])
                    piecerow++;
                if (piecerow == 3)
                    halfOpenFourCount++;
            }
        }
    }

    // 清算环节
    if ((openFourCount + halfOpenFourCount) > 1 || liveThreeCount > 1)
        return END; // 禁手生效，黑方负

    return GOON;
}

// 判断空格
int isempty(int x, int y)
{
    if (INBOARD(x) && INBOARD(y))
        if (arrayForInnerBoardLayout[x][y] == EMPTY)
            return 1;
    return 0;
}
// 判断是否为当前子
int iscurpiece(int x, int y, int curpiece)
{
    if (INBOARD(x) && INBOARD(y))
        if (arrayForInnerBoardLayout[x][y] == curpiece)
            return 1;
    return 0;
}