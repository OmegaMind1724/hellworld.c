#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define WIDTH 10
#define HEIGHT 20

// 方块形状
int shapes[7][4][4] = {
    {
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {1, 1, 1, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {1, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    {
        {1, 1, 1, 0},
        {0, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

// 游戏区域
int board[HEIGHT][WIDTH];

// 方块当前位置和形状
int x, y, shape, rotation;

// 初始化游戏区域
void initBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = 0;
        }
    }
}

// 生成新方块
void newPiece() {
    x = WIDTH / 2 - 2;
    y = 0;
    shape = rand() % 7;
    rotation = 0;
}

// 检查方块是否可以移动到指定位置
int canMove(int newX, int newY, int newRotation) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int rotatedX = i * (int)cos(newRotation * 90 * 3.1415926 / 180) - j * (int)sin(newRotation * 90 * 3.1415926 / 180);
            int rotatedY = i * (int)sin(newRotation * 90 * 3.1415926 / 180) + j * (int)cos(newRotation * 90 * 3.1415926 / 180);
            if (shapes[shape][rotatedX + 2][rotatedY + 2] == 1) {
                int newBoardX = newX + j;
                int newBoardY = newY + i;
                if (newBoardX < 0 || newBoardX >= WIDTH || newBoardY >= HEIGHT || (newBoardY >= 0 && board[newBoardY][newBoardX] == 1)) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// 合并方块到游戏区域
void mergePiece() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int rotatedX = i * (int)cos(rotation * 90 * 3.1415926 / 180) - j * (int)sin(rotation * 90 * 3.1415926 / 180);
            int rotatedY = i * (int)sin(rotation * 90 * 3.1415926 / 180) + j * (int)cos(rotation * 90 * 3.1415926 / 180);
            if (shapes[shape][rotatedX + 2][rotatedY + 2] == 1) {
                board[y + i][x + j] = 1;
            }
        }
    }
}

// 检查并消除满行
void checkLines() {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        int isFull = 1;
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == 0) {
                isFull = 0;
                break;
            }
        }
        if (isFull) {
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < WIDTH; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }
            i++;
        }
    }
}

// 绘制游戏区域
void drawBoard() {
    system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == 1) {
                printf("[]");
            } else {
                int isPartOfPiece = 0;
                for (int m = 0; m < 4; m++) {
                    for (int n = 0; n < 4; n++) {
                        int rotatedX = m * (int)cos(rotation * 90 * 3.1415926 / 180) - n * (int)sin(rotation * 90 * 3.1415926 / 180);
                        int rotatedY = m * (int)sin(rotation * 90 * 3.1415926 / 180) + n * (int)cos(rotation * 90 * 3.1415926 / 180);
                        if (shapes[shape][rotatedX + 2][rotatedY + 2] == 1 && x + n == j && y + m == i) {
                            isPartOfPiece = 1;
                            break;
                        }
                    }
                    if (isPartOfPiece) break;
                }
                if (isPartOfPiece) {
                    printf("[]");
                } else {
                    printf("  ");
                }
            }
        }
        printf("\n");
    }
}

// 主游戏循环
void gameLoop() {
    srand(time(NULL));
    initBoard();
    newPiece();
    while (1) {
        drawBoard();
        if (kbhit()) {
            char key = getch();
            if (key == 'a' && canMove(x - 1, y, rotation)) {
                x--;
            } else if (key == 'd' && canMove(x + 1, y, rotation)) {
                x++;
            } else if (key == 's' && canMove(x, y + 1, rotation)) {
                y++;
            } else if (key == 'w' && canMove(x, y, (rotation + 1) % 4)) {
                rotation = (rotation + 1) % 4;
            }
        }
        if (canMove(x, y + 1, rotation)) {
            y++;
        } else {
            mergePiece();
            checkLines();
            newPiece();
            if (!canMove(x, y, rotation)) {
                printf("Game Over!\n");
                break;
            }
        }
        Sleep(200);
    }
}

int main() {
    gameLoop();
    return 0;
}    