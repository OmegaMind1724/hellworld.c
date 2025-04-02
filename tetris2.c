#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 10
#define HEIGHT 20

int board[HEIGHT][WIDTH];
int currentPiece[4][4];
int pieceX, pieceY;
int pieceType;

// 初始化游戏板
void initBoard() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            board[y][x] = 0;
        }
    }
}

// 初始化当前方块
void initPiece() {
    pieceType = rand() % 7;
    pieceX = WIDTH / 2 - 2;
    pieceY = 0;

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            currentPiece[y][x] = 0;
        }
    }

    switch (pieceType) {
        case 0: // I 型
            currentPiece[0][0] = 1;
            currentPiece[0][1] = 1;
            currentPiece[0][2] = 1;
            currentPiece[0][3] = 1;
            break;
        case 1: // J 型
            currentPiece[0][0] = 1;
            currentPiece[1][0] = 1;
            currentPiece[1][1] = 1;
            currentPiece[1][2] = 1;
            break;
        case 2: // L 型
            currentPiece[0][2] = 1;
            currentPiece[1][0] = 1;
            currentPiece[1][1] = 1;
            currentPiece[1][2] = 1;
            break;
        case 3: // O 型
            currentPiece[0][0] = 1;
            currentPiece[0][1] = 1;
            currentPiece[1][0] = 1;
            currentPiece[1][1] = 1;
            break;
        case 4: // S 型
            currentPiece[0][1] = 1;
            currentPiece[0][2] = 1;
            currentPiece[1][0] = 1;
            currentPiece[1][1] = 1;
            break;
        case 5: // T 型
            currentPiece[0][1] = 1;
            currentPiece[1][0] = 1;
            currentPiece[1][1] = 1;
            currentPiece[1][2] = 1;
            break;
        case 6: // Z 型
            currentPiece[0][0] = 1;
            currentPiece[0][1] = 1;
            currentPiece[1][1] = 1;
            currentPiece[1][2] = 1;
            break;
    }
}

// 检查方块是否可以移动到指定位置
int canMove(int newX, int newY) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentPiece[y][x]) {
                int boardX = newX + x;
                int boardY = newY + y;
                if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT || (boardY >= 0 && board[boardY][boardX])) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// 将方块固定到游戏板上
void fixPiece() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (currentPiece[y][x]) {
                board[pieceY + y][pieceX + x] = 1;
            }
        }
    }
}

// 检查是否有满行并消除
void checkLines() {
    for (int y = HEIGHT - 1; y >= 0; y--) {
        int full = 1;
        for (int x = 0; x < WIDTH; x++) {
            if (!board[y][x]) {
                full = 0;
                break;
            }
        }
        if (full) {
            for (int yy = y; yy > 0; yy--) {
                for (int x = 0; x < WIDTH; x++) {
                    board[yy][x] = board[yy - 1][x];
                }
            }
            y++;
        }
    }
}

// 绘制游戏板
void drawBoard() {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (board[y][x]) {
                printf("*");
            } else {
                int inPiece = 0;
                for (int py = 0; py < 4; py++) {
                    for (int px = 0; px < 4; px++) {
                        if (currentPiece[py][px] && pieceX + px == x && pieceY + py == y) {
                            inPiece = 1;
                            break;
                        }
                    }
                    if (inPiece) break;
                }
                if (inPiece) {
                    printf("*");
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

// 旋转方块
void rotatePiece() {
    int rotated[4][4];
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            rotated[x][3 - y] = currentPiece[y][x];
        }
    }

    int canRotate = 1;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (rotated[y][x]) {
                int boardX = pieceX + x;
                int boardY = pieceY + y;
                if (boardX < 0 || boardX >= WIDTH || boardY >= HEIGHT || (boardY >= 0 && board[boardY][boardX])) {
                    canRotate = 0;
                    break;
                }
            }
        }
        if (!canRotate) break;
    }

    if (canRotate) {
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                currentPiece[y][x] = rotated[y][x];
            }
        }
    }
}

// 主游戏循环
void gameLoop() {
    srand(time(NULL));
    initBoard();
    initPiece();

    while (1) {
        drawBoard();

        if (kbhit()) {
            int key = getch();
            switch (key) {
                case 75: // 左箭头
                    if (canMove(pieceX - 1, pieceY)) {
                        pieceX--;
                    }
                    break;
                case 77: // 右箭头
                    if (canMove(pieceX + 1, pieceY)) {
                        pieceX++;
                    }
                    break;
                case 80: // 下箭头
                    if (canMove(pieceX, pieceY + 1)) {
                        pieceY++;
                    }
                    break;
                case 72: // 上箭头
                    rotatePiece();
                    break;
            }
        }

        if (canMove(pieceX, pieceY + 1)) {
            pieceY++;
        } else {
            fixPiece();
            checkLines();
            initPiece();
            if (!canMove(pieceX, pieceY)) {
                printf("Game Over!\n");
                break;
            }
        }

        Sleep(300);
    }
}

int main() {
    gameLoop();
    return 0;
}    