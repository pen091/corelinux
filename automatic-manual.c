// automator.c
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Step 1: Install tmux (Debian/Ubuntu)
    printf("Installing tmux...\n");
    system("sudo apt-get update && sudo apt-get install -y tmux");

    // Step 2: Install ngrok
    printf("Installing ngrok...\n");
    system("wget -q https://bin.equinox.io/c/bNyj1mQVY4c/ngrok-stable-linux-amd64.zip -O ngrok.zip");
    system("unzip -o ngrok.zip && rm ngrok.zip");
    system("sudo mv ngrok /usr/local/bin/");

    // Step 3: Add ngrok auth token (replace with your token!)
    system("ngrok config add-authtoken <YOUR_AUTH_TOKEN>");

    // Step 4: Create tmux session and split into 3 panels
    system("tmux new-session -d -s mysession");
    system("tmux split-window -h -t mysession");
    system("tmux split-window -v -t mysession:0.0");

    // Step 5: Run ngrok in first panel
    system("tmux send-keys -t mysession:0.0 'ngrok tcp 8080' C-m");

    // Step 6: Run server in second panel
    system("tmux send-keys -t mysession:0.1 './server' C-m");

    // Step 7: Run client in third panel (we still need ngrok port here)
    // For now, placeholder, will improve
    system("tmux send-keys -t mysession:0.2 './client 127.0.0.1 8080' C-m");

    // Attach to tmux
    system("tmux attach -t mysession");

    return 0;
}
