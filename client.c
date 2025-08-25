// client.c (supports hostname, e.g., 0.tcp.ngrok.io)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int sock;

// Thread function to receive messages from server
void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            printf("Server disconnected.\n");
            close(sock);
            exit(0);
        }
        printf("\nServer: %s\n> ", buffer);
        fflush(stdout);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server-hostname> <port>\n", argv[0]);
        return 1;
    }

    char *server_host = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in serv_addr;
    struct hostent *he;

    // Resolve hostname (e.g., 0.tcp.ngrok.io)
    if ((he = gethostbyname(server_host)) == NULL) {
        perror("gethostbyname error");
        exit(1);
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr, he->h_addr_list[0], he->h_length);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(1);
    }
    printf("Connected to %s:%d\n", server_host, port);

    // Create thread for receiving messages
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    // Main loop to send messages
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline
        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    return 0;
}
