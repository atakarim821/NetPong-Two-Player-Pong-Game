#ifndef GAME_H
#define GAME_H

#include <ncurses.h>

#define WIDTH  120 
#define HEIGHT 40 
#define OFFSETX 15 
#define OFFSETY 1 

typedef struct {
    int x, y;
    int dx, dy;
} Ball;

typedef struct {
    int x;
    int width;
} Paddle;

typedef struct {
    Paddle paddleA;
    Paddle paddleB;
    Ball ball;
    int scoreA;
    int scoreB;
    int is_server;
} GameState;

void init_game(GameState *game);
void draw_game(WINDOW *win, const GameState *game);
void move_ball(GameState *game);
void update_paddleA(GameState *game, int ch);
void update_paddleB(GameState *game, int ch);
void reset_ball(GameState *game);

#endif
