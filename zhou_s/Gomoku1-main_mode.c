#include "Gomoku.h"

// 棋盘使用的是GBK编码，每一个中文字符占用2个字节。

// 空棋盘模板
char arrayForEmptyBoard[SIZE][SIZE * CHARSIZE + 1] = //+1存\0
    {
        "┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
        "┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"};
// 此数组存储用于显示的棋盘
char arrayForDisplayBoard[SIZE][SIZE * CHARSIZE + 1];

char play1Pic[] = "●"; // 黑棋子;
char play1CurrentPic[] = "▲";

char play2Pic[] = "◎"; // 白棋子;
char play2CurrentPic[] = "△";

// 此数组用于记录当前的棋盘的格局
int arrayForInnerBoardLayout[SIZE][SIZE];

int row = 0, col = 0, sign = 1; // row,col为数组真正索引的行和列
// sign用于标记当前为黑棋子还是白棋，sign=-1为白棋，+1为黑棋

char input[6]; // 用于读取用户输入

int manpiece = BLACK;
int aipiece = WHITE; // 定义玩家和电脑的棋子

int main()

{
    int mode = 0, firstplayer = 0;
    while (1)
    {
        printf("人机对战，请输入1\n");
        printf("人人对战，请输入2：\n");
        scanf("%d", &mode);
        getchar();
        if (mode == 1 || mode == 2)
            break;
        else
        {
            printf("输入错误，请重新输入！\n");
            continue;
        }
    }
    initRecordBoard(); // 初始化一个空棋盘并显示
    innerLayoutToDisplayArray();
    displayBoard();

    int isful = 0; // 判断是否填满棋盘
    sign = 1;      // 先手黑棋
    int state = 0; // 判断下棋过程中的情况

    if (mode == 1)
    {
        while (1)
        {
            printf("电脑先手，请输入1\n");
            printf("玩家先手，请输入2：\n");
            scanf("%d", &firstplayer);
            getchar();
            if (firstplayer == 1 || firstplayer == 2)
                break;
            else
            {
                printf("输入错误，请重新输入！\n");
                continue;
            }
        }
        manpiece = (firstplayer == 1) ? WHITE : BLACK;
        aipiece = (firstplayer == 2) ? WHITE : BLACK;
        while (1)
        {
            if (((sign > 0) ? BLACK : WHITE) == manpiece)
            {
                state = ManGo(); // 人类下棋
                if (state == QUIT)
                    return 0;
                else if (state == WRONG)
                    continue;
            }
            else
                AiGo(); // 电脑下棋

            innerLayoutToDisplayArray(); // 显示棋盘
            displayBoard();

            // 判断胜负、禁手和平局
            if (checkwin())
            {
                printf("《%s》胜利!!!!!\n", sign > 0 ? "黑方" : "白方");
                return 0;
            }
            if (sign > 0 && checkForbiddenMoves(row, col))
            {
                printf("触犯禁手，黑方负!!!!!\n如果是误判，请输入pass忽略本次判断\n");
                getinput(input);
                if (input[0] != 'p')
                    return 0;
                printf("游戏继续!!!\n");
            }
            isful++;
            if (isful >= SIZE * SIZE)
            {
                printf("平局！！！\n");
                return 0;
            }
            sign *= -1; // 切换下一轮的棋子
        }
    }
    else if (mode == 2)
    {
        while (1)
        {
            manpiece = (sign > 0) ? BLACK : WHITE;
            state = ManGo(); // 人类下棋
            if (state == QUIT)
                return 0;
            else if (state == WRONG)
                continue;

            innerLayoutToDisplayArray(); // 显示棋盘
            displayBoard();

            // 判断胜负、禁手和平局
            if (checkwin())
            {
                printf("《%s》胜利!!!!!\n", sign > 0 ? "黑方" : "白方");
                return 0;
            }
            if (sign > 0 && checkForbiddenMoves(row, col))
            {
                printf("触犯禁手，黑方负!!!!!\n如果是误判，请输入pass忽略本次判断\n");
                getinput(input);
                if (input[0] != 'p')
                    return 0;
                printf("游戏继续!!!\n");
            }
            isful++;
            if (isful >= SIZE * SIZE)
            {
                printf("平局！！！\n");
                return 0;
            }
            sign *= -1; // 切换下一轮的棋子
        }
    }

    return 0;
}
// 初始化一个空棋盘格局
void initRecordBoard(void)
{
    // 通过双重循环，将arrayForInnerBoardLayout清0
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
            arrayForInnerBoardLayout[i][j] = EMPTY;
    return;
}

// 将arrayForInnerBoardLayout中记录的棋子位置，转化到arrayForDisplayBoard中
void innerLayoutToDisplayArray(void)
{
    // 第一步：将arrayForEmptyBoard中记录的空棋盘，复制到arrayForDisplayBoard中
    int i, j;
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE * CHARSIZE + 1; j++)
            arrayForDisplayBoard[i][j] = arrayForEmptyBoard[i][j];

    // 第二步：扫描arrayForInnerBoardLayout，当遇到非EMPTY的元素，将●或者◎复制到arrayForDisplayBoard的相应位置上
    for (i = 0; i < SIZE; i++)
        for (j = 0; j < SIZE; j++)
        {
            if (arrayForInnerBoardLayout[i][j] == BLACK)
            {
                arrayForDisplayBoard[i][j * CHARSIZE] = play1Pic[0];
                arrayForDisplayBoard[i][j * CHARSIZE + 1] = play1Pic[1];
                if (CHARSIZE == 3)
                    arrayForDisplayBoard[i][j * CHARSIZE + 2] = play1Pic[2];
            }
            else if (arrayForInnerBoardLayout[i][j] == WHITE)
            {
                arrayForDisplayBoard[i][j * CHARSIZE] = play2Pic[0];
                arrayForDisplayBoard[i][j * CHARSIZE + 1] = play2Pic[1];
                if (CHARSIZE == 3)
                    arrayForDisplayBoard[i][j * CHARSIZE + 3] = play2Pic[2];
            }
        }
    // 第三步：把最新的棋子换成△或▲
    i = row;
    j = col;
    if (arrayForInnerBoardLayout[i][j] == BLACK)
    {
        arrayForDisplayBoard[i][j * CHARSIZE] = play1CurrentPic[0];
        arrayForDisplayBoard[i][j * CHARSIZE + 1] = play1CurrentPic[1];
        if (CHARSIZE == 3)
            arrayForDisplayBoard[i][j * CHARSIZE + 3] = play1CurrentPic[3];
    }
    else if (arrayForInnerBoardLayout[i][j] == WHITE)
    {
        arrayForDisplayBoard[i][j * CHARSIZE] = play2CurrentPic[0];
        arrayForDisplayBoard[i][j * CHARSIZE + 1] = play2CurrentPic[1];
        if (CHARSIZE == 3)
            arrayForDisplayBoard[i][j * CHARSIZE + 3] = play2CurrentPic[3];
    }
}

// 显示棋盘格局
void displayBoard(void)
{
    int i;
    // 第一步：清屏
    system("clear"); // 清屏
    // 第二步：将arrayForDisplayBoard输出到屏幕上
    for (i = 0; i < SIZE; i++)
        printf("%3d %s\n", SIZE - i, arrayForDisplayBoard[i]);

    // 第三步：输出最下面的一行字母A B ....
    char c = 'A';
    printf("    ");
    for (i = 0; i < SIZE; i++)
    {
        printf("%2c", c);
        c++;
    }
    printf("\n");
}

// 读取用户输入
int getinput(char v[])
{
    int i = 0;
    char c;
    while ((c = getchar()) != EOF && c != '\n' && i < 5)
        if (isalnum(c))
            v[i++] = c;
    v[i] = '\0';
    return strlen(v);
}