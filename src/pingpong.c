#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "game.h"
#include "net.h"

#define TIMEOUT_MS 10 

static GameState game;
static int game_running = 1;
static int sockfd;
static pthread_mutex_t game_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Shared networking functions from net.c */
//extern int setup_server_socket(int port);
//extern int setup_client_socket(const char *server_ip, int port);
//extern ssize_t send_all(int sockfd, const void *buf, size_t len);
//extern ssize_t recv_all(int sockfd, void *buf, size_t len);

void* ball_thread(void *arg) {
    while (game_running) {
        usleep(80000);
        pthread_mutex_lock(&game_mutex);
        move_ball(&game);
        
        MessageType type = MSG_GAME_STATE;
        send_all(sockfd, &type, sizeof(MessageType));
        send_all(sockfd, &game, sizeof(GameState));
        
        pthread_mutex_unlock(&game_mutex);
    }
    return NULL;
}

void* receive_thread(void *arg) {
    while (game_running) {
        MessageType type;
        if (recv_all(sockfd, &type, sizeof(MessageType)) <= 0)
            break;

        if (type == MSG_PADDLE_UPDATE) {
            int remote_paddle;
            recv_all(sockfd, &remote_paddle, sizeof(int));
            pthread_mutex_lock(&game_mutex);
            if(game.is_server) game.paddleB.x = remote_paddle;
            else game.paddleA.x = remote_paddle;
            pthread_mutex_unlock(&game_mutex);
        }
        else if (type == MSG_GAME_STATE) {
            GameState remote_state;
            recv_all(sockfd, &remote_state, sizeof(GameState));
            pthread_mutex_lock(&game_mutex);
           if(game.is_server) {
               memcpy(&game.paddleB, &remote_state.paddleB, sizeof(Paddle));
           } else {
               memcpy(&game.paddleA, &remote_state.paddleA, sizeof(Paddle));
               memcpy(&game.ball, &remote_state.ball, sizeof(Ball));
               game.scoreA = remote_state.scoreA;
               game.scoreB = remote_state.scoreB;
           }
            pthread_mutex_unlock(&game_mutex);
        }
    }
    game_running = 0;
    return NULL;
}

void init_ncurses() {
    initscr();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
    timeout(TIMEOUT_MS);
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    noecho();
}

int main(int argc, char *argv[]) {
    if ((argc != 3 && strcmp(argv[1], "server") == 0) ||
        (argc != 4 && strcmp(argv[1], "client") == 0)) {
        fprintf(stderr, "Usage:\n  Server: %s server <PORT>\n  Client: %s client <SERVER_IP> <PORT>\n",
                argv[0], argv[0]);
        exit(1);
    }

    /* Initialize game state */
    game.is_server = (strcmp(argv[1], "server") == 0);
    init_game(&game);

    /* Network setup */
    if(game.is_server) {
        sockfd = setup_server_socket(atoi(argv[2])); //  
    } else {
        sockfd = setup_client_socket(argv[2], atoi(argv[3]));
    }

    init_ncurses();
    pthread_t ball_tid, recv_tid;

    if(game.is_server) {
        pthread_create(&ball_tid, NULL, ball_thread, NULL);
    }

    //pthread_create(&ball_tid, NULL, ball_thread, NULL);
    pthread_create(&recv_tid, NULL, receive_thread, NULL);

    while (game_running) {
        int ch = getch();
        if (ch == 'q') {
            game_running = 0;
            break;
        }
        
        /* Handle paddle movement */
        if(ch == KEY_LEFT || ch == KEY_RIGHT) {
            pthread_mutex_lock(&game_mutex);
            
            if(game.is_server) {
                update_paddleA(&game, ch);
            } else {
                update_paddleB(&game, ch);
            }

            /* Send paddle update */
            MessageType type = MSG_PADDLE_UPDATE;
            int paddle_pos = game.is_server ? game.paddleA.x : game.paddleB.x;
            send_all(sockfd, &type, sizeof(MessageType));
            send_all(sockfd, &paddle_pos, sizeof(int));
            
            pthread_mutex_unlock(&game_mutex);
        }

        /* Draw game state */
        pthread_mutex_lock(&game_mutex);
        draw_game(stdscr, &game);
        pthread_mutex_unlock(&game_mutex);
    }

    /* Cleanup */
    
    if(game.is_server) pthread_join(ball_tid, NULL);
    //pthread_join(ball_tid, NULL);
    pthread_join(recv_tid, NULL);
    endwin();
    close(sockfd);
    return 0;
}
