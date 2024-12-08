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

typedef struct Character {    
    int x, y, size_x, size_y;
    char sprite[9];
} plane;

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