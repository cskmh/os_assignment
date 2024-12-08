#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ARROW 224
#define SPACE 32
#define R 82
#define r 114
#define MapXMax 80
#define MapYMax 55

#define MaxBullet 10
#define MaxEnemy 10

int b_index = 0;
int i_index = 0;
int count = 0;
int eb_index = 0;
int lives = 3; // 플레이어 생명
char mapData[MapYMax * MapXMax];
const int mapValue = MapYMax * MapXMax;  // 맵 크기 정의

void FillMap(char str[], char str_s, int max_value) {    
    for (int i = 0; i < max_value; i++) {
        str[i] = str_s;
    }
}

void EditMap(int x, int y, char str) { 
    if (x >= 0 && y >= 0 && x < MapXMax && y < MapYMax) { // 좌표 검증
        mapData[y * MapXMax + x] = str; // 맵 데이터에 값 쓰기
    }
}

void DrawSprite(int x, int y, int size_x, int size_y, const char spr[]) { 
    for (int i = 0; i < size_y; i++) {
        for (int j = 0; j < size_x; j++) {
            EditMap(x + j, y + i, spr[i * size_x + j]);
        }
    }
typedef struct Character {    
    int x, y, size_x, size_y;
    char sprite[9];
} plane;

void controlcharacter(plane* pPlane) { 8
    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        if (pPlane->y > 0) pPlane->y--;
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        if (pPlane->y < MapYMax - pPlane->size_y) pPlane->y++;
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        if (pPlane->x > 0) pPlane->x--;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        if (pPlane->x < MapXMax - pPlane->size_x) pPlane->x++;
    }
}

int main(void) {
    system("title Flight Game");
    system("mode con:cols=80 lines=60");
    CursorView(0);
    srand(time(NULL));

    plane myPlane;
    myPlane.x = MapXMax / 2;
    myPlane.y = MapYMax - 5;
    myPlane.size_x = 3;
    myPlane.size_y = 2;
    strcpy(myPlane.sprite, "_^_");

    while (1) {
        if (lives <= 0) {
            GotoXY(30, 25);
            printf("Game Over! Press any key to exit...");
            _getch();
            break;
        }

        FillMap(mapData, ' ', mapValue);
        DrawSprite(myPlane.x, myPlane.y, myPlane.size_x, myPlane.size_y, myPlane.sprite);
        Bullet_Shoot(Bullet_info, &myPlane);
        Enemy_Create(Enemies, count++);
        Enemy_Move(Enemies);
        Enemy_Attack(Enemy_Bullet, Enemies, &myPlane);
        ShowOption();

        GotoXY(0, 0);
        for (int i = 0; i < MapYMax; i++) {
            for (int j = 0; j < MapXMax; j++) {
                putchar(mapData[i * MapXMax + j]);
            }
            putchar('\n');
        }

        controlcharacter(&myPlane);
        Sleep(50);
    }
    return 0;
}