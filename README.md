gcc -std=c99 dodger_simple.c -o dodger_simple.exe
gcc -std=c99 upgraded_dodger.c -o upgraded_dodger.exe
# then run:
.\upgraded_dodger.exe
.\dodger_simple.exe
Left / Right arrow keys — move left / right

A / D — alternative left / right (upgraded)

P — Pause (upgraded)

Q — Quit (upgraded)

Both use <conio.h> and <windows.h> — Windows only.

Short comparison — How Sir's code differs from yours

Multiple obstacles vs single obstacle

Your (simple): one obstacle moving down a lane.

Sir (upgraded): obstacle pool (MAX_OBS) with random spawns and multiple on-screen obstacles.

Scoring & progression

Your: no scoring/levels.

Sir: score increments when obstacles pass, level increases every LEVEL_STEP points, game speed (delay) reduces as level rises.

Lives / Game over

Your: immediate game over on collision.

Sir: lives (multiple hits allowed) and better end-screen message.

Controls & UX

Sir: supports arrow keys + A/D, P to pause, Q to quit, cursor hiding, clearer HUD (Score, Lives, Level).

Obstacle lifecycle

Your: obstacle resets on reaching bottom.

Sir: obstacle slot recycling, spawn chance (SPAWN_CHANCE) and pass-detection to award points.

Polish & maintainability

Sir: organized obstacle struct, configurable macros (MAX_OBS, BASE_DELAY, etc.), helper functions (hideCursor, setCursorPosition).

Sir: prepared for smoother drawing (uses console cursor functions) and has clearer HUD layout.

Difficulty tuning

Sir: constants to tune spawn rate, delay, min delay, level step — easier to tweak difficulty.

Notes / Next steps (optional)

Replace system("cls") / Sleep() with partial redraws using SetConsoleCursorPosition() to reduce flicker.

Add high-score save/load, sound (Beep()), or port to cross-platform (ncurses) if needed.
