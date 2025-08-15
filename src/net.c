#include "net.h"
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int setup_server_socket(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(port)
    };
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 1);
    int client_fd = accept(server_fd, NULL, NULL);
    close(server_fd);
    return client_fd;
}

int setup_client_socket(const char *server_ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    inet_pton(AF_INET, server_ip, &addr.sin_addr);
    connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    return sock;
}

ssize_t send_all(int sockfd, const void *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t sent = write(sockfd, (char*)buf + total, len - total);
        if (sent <= 0) return sent;
        total += sent;
    }
    return total;
}

ssize_t recv_all(int sockfd, void *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t received = read(sockfd, (char*)buf + total, len - total);
        if (received <= 0) return received;
        total += received;
    }
    return total;
}
