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

typedef struct Bullet { 
    int x, y;
    bool Active;
} Bullet;

Bullet Bullet_info[MaxBullet]; 
Bullet Enemy_Bullet[MaxBullet]; 

void Bullet_Shoot(Bullet* pBullet, plane* pPlane) { 
    for (int i = 0; i < MaxBullet; i++) {
        if (pBullet[i].Active) {
            pBullet[i].y--;
            if (pBullet[i].y < 0) {
                pBullet[i].Active = false;
            }
            else {
                EditMap(pBullet[i].x, pBullet[i].y, '*');

                // 적 비행기와 충돌 처리
                for (int j = 0; j < MaxEnemy; j++) {
                    if (Enemies[j].Active &&
                        pBullet[i].x >= Enemies[j].x && pBullet[i].x < Enemies[j].x + 3 &&
                        pBullet[i].y == Enemies[j].y) {
                        Enemies[j].Active = false; // 적 비활성화
                        pBullet[i].Active = false; // 총알 비활성화
                    }
                }
            }
        }
    }

    if (GetAsyncKeyState(SPACE) & 0x8000) { // SPACE 키로 발사
        if (b_index < MaxBullet) {
            pBullet[b_index].x = pPlane->x + 1;
            pBullet[b_index].y = pPlane->y - 1;
            pBullet[b_index].Active = true;
            b_index++;
        }
    }

    if (GetAsyncKeyState(R) & 0x8000) { // R 키로 재장전
        b_index = 0;
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