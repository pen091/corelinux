// automator.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 2048

// Function to extract hostname and port from ngrok API JSON
void get_ngrok_addr(char *host, char *port) {
    FILE *fp;
    char buffer[BUFFER_SIZE];

    // Run curl to fetch API response
    fp = popen("curl -s http://127.0.0.1:4040/api/tunnels", "r");
    if (fp == NULL) {
        perror("Failed to run curl");
        exit(1);
    }

    fread(buffer, 1, sizeof(buffer)-1, fp);
    pclose(fp);

    // Look for "tcp://"
    char *start = strstr(buffer, "tcp://");
    if (!start) {
        fprintf(stderr, "No TCP tunnel found. Is ngrok running?\n");
        exit(1);
    }

    start += 6; // skip "tcp://"
    char *colon = strchr(start, ':');
    if (!colon) {
        fprintf(stderr, "Failed to parse host\n");
        exit(1);
    }

    strncpy(host, start, colon - start);
    host[colon - start] = '\0';

    char *end = strchr(colon+1, '"');
    strncpy(port, colon+1, end - (colon+1));
    port[end - (colon+1)] = '\0';
}

int main() {
    char host[256], port[16], client_cmd[512];

    // Step 1: Install tmux + ngrok (Debian/Ubuntu)
    system("sudo apt-get update -y && sudo apt-get install -y tmux curl unzip");
    system("wget -q https://bin.equinox.io/c/bNyj1mQVY4c/ngrok-stable-linux-amd64.zip -O ngrok.zip");
    system("unzip -o ngrok.zip && rm ngrok.zip");
    system("sudo mv ngrok /usr/local/bin/");

    // Step 2: Add ngrok token (replace with your token!)
    system("ngrok config add-authtoken <YOUR_AUTH_TOKEN>");

    // Step 3: Create tmux session with 3 panels
    system("tmux new-session -d -s mysession");
    system("tmux split-window -h -t mysession");
    system("tmux split-window -v -t mysession:0.0");

    // Step 4: Start ngrok tcp 8080 in panel 0
    system("tmux send-keys -t mysession:0.0 'ngrok tcp 8080' C-m");
    sleep(5); // wait for ngrok to start

    // Step 5: Fetch ngrok hostname + port
    get_ngrok_addr(host, port);
    printf("Ngrok forwarding: %s:%s\n", host, port);

    // Step 6: Start server in panel 1
    system("tmux send-keys -t mysession:0.1 './server' C-m");

    // Step 7: Start client in panel 2 with ngrok forwarding
    snprintf(client_cmd, sizeof(client_cmd), "tmux send-keys -t mysession:0.2 './client %s %s' C-m", host, port);
    system(client_cmd);

    // Attach to tmux so user sees it
    system("tmux attach -t mysession");

    return 0;
}
