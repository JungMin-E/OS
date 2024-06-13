#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 2048

int sock = 0;
char client_id[BUFFER_SIZE];

void handle_exit_signal(int sig) {
    char exit_message[BUFFER_SIZE];
    snprintf(exit_message, sizeof(exit_message), "%s has exited\n", client_id);
    send(sock, exit_message, strlen(exit_message), 0);
    printf("Exiting...\n");
    close(sock);
    exit(0);
}

void *receive_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    int read_size;
    while ((read_size = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_size] = '\0';
        printf("%s", buffer);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    pthread_t recv_thread;
    const char *server_ip = "172.17.0.2"; // 서버 IP 주소

    snprintf(client_id, sizeof(client_id), "client1");

    signal(SIGINT, handle_exit_signal);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }

    printf("Connected to server\n");

    pthread_create(&recv_thread, NULL, receive_handler, NULL);

    char message[BUFFER_SIZE];
    char send_buffer[BUFFER_SIZE + 50];
    while (fgets(message, BUFFER_SIZE, stdin) != NULL) {
        snprintf(send_buffer, sizeof(send_buffer), "%s: %s", client_id, message);
        send(sock, send_buffer, strlen(send_buffer), 0);
    }

    close(sock);
    return 0;
}
