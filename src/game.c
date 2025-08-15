#include "game.h"
#include <pthread.h>
#include <unistd.h>

void init_game(GameState *game) {
    game->paddleA.x = WIDTH / 2 - 3;
    game->paddleA.width = 18;
    game->paddleB.x = WIDTH / 2 - 3;
    game->paddleB.width = 18;
    game->ball.x = WIDTH / 2;
    game->ball.y = HEIGHT / 2;
    game->ball.dx = 1;
    game->ball.dy = 1;
    game->scoreA = 0;
    game->scoreB = 0;
}

void draw_game(WINDOW *win, const GameState *game) {
    clear();

    attron(COLOR_PAIR(1));
    for (int i = OFFSETX; i <= OFFSETX + WIDTH; i++) {
        mvprintw(OFFSETY - 1, i, " ");
    }
    mvprintw(OFFSETY - 1, OFFSETX + 3, "CS3205 NetPong, Ball: %d, %d", game->ball.x, game->ball.y);
    mvprintw(OFFSETY - 1, OFFSETX + WIDTH - 25, "Player A: %d, Player B: %d", game->scoreA, game->scoreB);

    for (int i = OFFSETY; i < OFFSETY + HEIGHT; i++) {
        mvprintw(i, OFFSETX, "  ");
        mvprintw(i, OFFSETX + WIDTH -1 , "  ");
    }
    for (int i = OFFSETX; i < OFFSETX + WIDTH; i++) {
        mvprintw(OFFSETY, i, " ");
        mvprintw(OFFSETY + HEIGHT - 1, i, " ");
    }
    attroff(COLOR_PAIR(1));

    mvprintw(OFFSETY + game->ball.y, OFFSETX + game->ball.x, "o");

    attron(COLOR_PAIR(2));
    for (int i = 0; i < game->paddleA.width; i++) {
        mvprintw(OFFSETY + HEIGHT - 2, OFFSETX + game->paddleA.x + i, " ");
    }
    for (int i = 0; i < game->paddleB.width; i++) {
        mvprintw(OFFSETY + 1, OFFSETX + game->paddleB.x + i, " ");
    }
    attroff(COLOR_PAIR(2));

    refresh();
}

void move_ball(GameState *game) {
    game->ball.x += game->ball.dx;
    game->ball.y += game->ball.dy;

    if (game->ball.y <= 0) {
        game->scoreA++;
        reset_ball(game);
        return;
    }
    if (game->ball.y >= HEIGHT - 1) {
        game->scoreB++;
        reset_ball(game);
        return;
    }

    if (game->ball.x <= 0 || game->ball.x >= WIDTH - 1) {
        game->ball.dx = -game->ball.dx;
    }

    if (game->ball.y == 1 && game->ball.dy < 0) {
        if (game->ball.x >= game->paddleB.x && game->ball.x < game->paddleB.x + game->paddleB.width) {
            game->ball.dy = -game->ball.dy;
        }
    }
    if (game->ball.y == HEIGHT - 2 && game->ball.dy > 0) {
        if (game->ball.x >= game->paddleA.x && game->ball.x < game->paddleA.x + game->paddleA.width) {
            game->ball.dy = -game->ball.dy;
        }
    }
}

void update_paddleA(GameState *game, int ch) {
    if (ch == KEY_LEFT && game->paddleA.x > 0) {
        game->paddleA.x--;
    }
    if (ch == KEY_RIGHT && game->paddleA.x < WIDTH - game->paddleA.width) {
        game->paddleA.x++;
    }
}

void update_paddleB(GameState *game, int ch) {
    if (ch == KEY_LEFT && game->paddleB.x > 0) {
        game->paddleB.x--;
    }
    if (ch == KEY_RIGHT && game->paddleB.x < WIDTH - game->paddleB.width) {
        game->paddleB.x++;
    }
}

void reset_ball(GameState *game) {
    game->ball.x = WIDTH / 2;
    game->ball.y = HEIGHT / 2;
    game->ball.dx = 1;
    game->ball.dy = 1;
}
