#include "Gomoku.h"
int ManGo()
{

    int i, j;
    row = 0, col = 0;
    printf("玩家《%s》请输入位置：\n", sign > 0 ? "黑方" : "白方");
    getinput(input);
    // 将输入转化为准确位置
    // 用户输入的值直接存到row，col中,再用size-row
    for (i = 0; input[i] != '\0'; i++)
    {
        if (input[i] >= 'a' && input[i] <= 'o')
            col = input[i] - 'a';
        else if (input[i] >= 'A' && input[i] <= 'O')
            col = input[i] - 'A';
        else if (isdigit(input[i]))
        {
            row = row * 10 + input[i] - '0';
        }
        else if (input[i] == 'q')
            return QUIT; // 表示退出游戏
        else
        {
            printf("输入有误!!!\n");
            return WRONG; // 表示输入有误，退出函数，重新循环
        }
    }
    row = SIZE - row;
    if (row >= SIZE || col >= SIZE || row < 0 || col < 0 || arrayForInnerBoardLayout[row][col] != EMPTY)
    {
        printf("输入有误!!!\n");
        return WRONG;
    }
    arrayForInnerBoardLayout[row][col] = manpiece;
    return DONE; // 成功下棋
}