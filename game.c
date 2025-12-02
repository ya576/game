// upgraded_dodger.c
// Compile on Windows (Visual Studio / MinGW)
// Features: multiple obstacles, score, levels, lives, pause, arrow-key support

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define LANES 3
#define HEIGHT 12       // visible rows above the player
#define MAX_OBS 6       // max obstacles on screen
#define SPAWN_CHANCE 30 // percent chance to spawn each tick (0-99)
#define SCORE_PER_PASS 1
#define LEVEL_STEP 10   // points needed to increase level
#define BASE_DELAY 140  // initial frame delay (ms)
#define MIN_DELAY 40

typedef struct {
    int active;   // 0 = none, 1 = active
    int lane;     // 0..LANES-1
    int y;        // 0..HEIGHT-1 (0 = top)
} Obstacle;

void setCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE; // set the cursor visibility
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

int main() {
    srand((unsigned)time(NULL));
    hideCursor();

    int playerLane = 1; // start middle (0..2)
    Obstacle obs[MAX_OBS] = {0};
    int score = 0;
    int lives = 3;
    int level = 1;
    int delay = BASE_DELAY;
    int tick = 0;
    int paused = 0;

    // Welcome / controls
    printf("Console Dodger - Next Level\n");
    printf("Controls: Left/Right arrows or A/D to move. P = Pause, Q = Quit.\n");
    printf("Avoid obstacles. Score increases when obstacles pass.\n");
    printf("Press any key to start...\n");
    getch();

    // game loop
    while (1) {
        // input (non-blocking)
        if (_kbhit()) {
            int ch = getch();
            if (ch == 0 || ch == 224) { // arrow or extended
                int ch2 = getch();
                if (ch2 == 75 && playerLane > 0) playerLane--; // left
                else if (ch2 == 77 && playerLane < LANES-1) playerLane++; // right
            } else {
                // letter keys
                if (ch == 'a' || ch == 'A') { if (playerLane > 0) playerLane--; }
                if (ch == 'd' || ch == 'D') { if (playerLane < LANES-1) playerLane++; }
                if (ch == 'p' || ch == 'P') paused = !paused;
                if (ch == 'q' || ch == 'Q') break;
            }
        }

        if (!paused) {
            // spawn new obstacle with some probability
            if (rand() % 100 < SPAWN_CHANCE) {
                for (int i = 0; i < MAX_OBS; i++) {
                    if (!obs[i].active) {
                        obs[i].active = 1;
                        obs[i].lane = rand() % LANES;
                        obs[i].y = 0; // top
                        break;
                    }
                }
            }

            // move obstacles down
            for (int i = 0; i < MAX_OBS; i++) {
                if (obs[i].active) {
                    obs[i].y++; // move down

                    // if reached below player (passed)
                    if (obs[i].y > HEIGHT) {
                        obs[i].active = 0;
                        score += SCORE_PER_PASS;
                        // level up logic
                        int newLevel = score / LEVEL_STEP + 1;
                        if (newLevel > level) {
                            level = newLevel;
                            // speed up
                            delay = BASE_DELAY - (level - 1) * 12;
                            if (delay < MIN_DELAY) delay = MIN_DELAY;
                        }
                    }
                }
            }

            // collision check: when obstacle y == HEIGHT (player row), same lane -> hit
            for (int i = 0; i < MAX_OBS; i++) {
                if (obs[i].active && obs[i].y == HEIGHT && obs[i].lane == playerLane) {
                    lives--;
                    obs[i].active = 0; // remove obstacle
                    // small "invuln" could be added; for now immediate effect
                    if (lives <= 0) {
                        // draw final screen
                        system("cls");
                        printf("====== GAME OVER ======\n");
                        printf("Score: %d   Level: %d\n", score, level);
                        printf("Press any key to exit...\n");
                        getch();
                        return 0;
                    }
                }
            }
        }

        // ---- DRAW ----
        // We'll clear and redraw. For smoother visuals consider double-buffering or partial redraws.
        system("cls");

        // HUD
        printf("Score: %d   Lives: %d   Level: %d   %s\n", score, lives, level, paused ? "(PAUSED - P to resume)" : "");
        printf("+---+---+---+\n");

        // draw HEIGHT rows (0..HEIGHT-1), obstacles when obs.y == row
        for (int row = 0; row <= HEIGHT; row++) {
            // row 'HEIGHT' is the player row (bottom)
            if (row < HEIGHT) {
                // prepare strings per lane
                char laneStr[LANES][8];
                for (int ln = 0; ln < LANES; ln++) laneStr[ln][0] = ' ', laneStr[ln][1] = '\0';

                // check obstacles
                for (int i = 0; i < MAX_OBS; i++) {
                    if (obs[i].active && obs[i].y == row) {
                        // use 'O' as obstacle
                        laneStr[obs[i].lane][0] = 'O';
                        laneStr[obs[i].lane][1] = '\0';
                    }
                }

                // print row
                printf("| %s | %s | %s |\n",
                    laneStr[0][0] ? laneStr[0] : " ",
                    laneStr[1][0] ? laneStr[1] : " ",
                    laneStr[2][0] ? laneStr[2] : " ");
            } else {
                // player row
                char p0 = ' ', p1 = ' ', p2 = ' ';
                if (playerLane == 0) p0 = 'A'; // player icon
                else if (playerLane == 1) p1 = 'A';
                else if (playerLane == 2) p2 = 'A';

                printf("| %c | %c | %c |\n", p0, p1, p2);
            }
        }

        // small delay representing frame rate
        Sleep(delay);

        tick++;
    }

    // cleanup / exit
    printf("\nThanks for playing! Final score: %d   Level: %d\n", score, level);
    return 0;
}
