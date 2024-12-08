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

typedef struct Enemy { 
    int x, y;
    bool Active;
    char sprite[5];
} Enemy;

Enemy Enemies[MaxEnemy]; 

void Enemy_Create(Enemy* Enemies, int count) { 
    if (count % 200 == 0) {
        int a = rand() % 60 + 10;
        int b = rand() % 10 + 5;
        Enemies[i_index].x = a;
        Enemies[i_index].y = b;
        Enemies[i_index].Active = true;
        strncpy(Enemies[i_index].sprite, "^O^ ", sizeof(Enemies[i_index].sprite)); // 2x1 크기
        i_index = (i_index + 1) % MaxEnemy;
    }

    for (int i = 0; i < MaxEnemy; i++) {
        if (Enemies[i].Active) {
            DrawSprite(Enemies[i].x, Enemies[i].y, 3, 1, Enemies[i].sprite);
        }
    }
}

void Enemy_Move(Enemy* Enemies) { 
    for (int i = 0; i < MaxEnemy; i++) {
        if (!Enemies[i].Active) continue;

        if (rand() % 4 == 0) {
            Enemies[i].x += rand() % 2 == 0 ? 1 : -1;
        }

        if (Enemies[i].x < 0) Enemies[i].x = 0;
        if (Enemies[i].x >= MapXMax - 3) Enemies[i].x = MapXMax - 3; // 적 크기 3 고려
        if (Enemies[i].y < 0) Enemies[i].y = 0;
        if (Enemies[i].y >= MapYMax / 2) Enemies[i].y = MapYMax / 2 - 1;
    }
}

void Enemy_Attack(Bullet* Enemy_Bullet, Enemy* Enemies, plane* pPlane) { 
    for (int i = 0; i < MaxBullet; i++) {
        if (Enemy_Bullet[i].Active) {
            Enemy_Bullet[i].y++;
            if (Enemy_Bullet[i].y >= MapYMax) {
                Enemy_Bullet[i].Active = false;
            }
            else {
                EditMap(Enemy_Bullet[i].x, Enemy_Bullet[i].y, '|');

                // 플레이어와 충돌 감지
                if (Enemy_Bullet[i].x >= pPlane->x && Enemy_Bullet[i].x < pPlane->x + pPlane->size_x &&
                    Enemy_Bullet[i].y >= pPlane->y && Enemy_Bullet[i].y < pPlane->y + pPlane->size_y) {
                    Enemy_Bullet[i].Active = false;
                    lives--; // 생명 감소
                }
            }
        }
    }

    if (rand() % 10 != 0) return;

    if (eb_index >= MaxBullet) eb_index = 0;

    for (int i = 0; i < MaxEnemy; i++) {
        if (Enemies[i].Active) {
            Enemy_Bullet[eb_index].x = Enemies[i].x + 1;
            Enemy_Bullet[eb_index].y = Enemies[i].y + 1;
            Enemy_Bullet[eb_index].Active = true;
            eb_index++;
        }
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