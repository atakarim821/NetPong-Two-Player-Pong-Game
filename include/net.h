#ifndef NET_H
#define NET_H

#include <sys/socket.h>
#include <netinet/in.h>

typedef enum {
    MSG_PADDLE_UPDATE,
    MSG_GAME_STATE
} MessageType;

int setup_server_socket(int port);
int setup_client_socket(const char *server_ip, int port);
ssize_t send_all(int sockfd, const void *buf, size_t len);
ssize_t recv_all(int sockfd, void *buf, size_t len);

#endif
