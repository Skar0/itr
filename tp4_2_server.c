#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main() {

    signal(SIGCHLD, SIG_IGN); //On ignore les fils donc ils ne deviennent pas zombie
    //Creation du socket
    int listen_fd = socket(PF_INET, SOCK_STREAM, 0); //Creation d'un socket IP (PF_INET) + TCP (le flag 0) et mise de son descripteur de fichier dans listen_fd
    
    if (listen_fd == -1) {
       perror("Failed to create socket");
    } 
    
    //Adresse IPV4 d'écoute
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(4237);
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //Récupère l'adresse locale
    
    //Lie l'adresse
    if(bind (listen_fd, (const struct sockaddr*) &addr, sizeof(addr)) == -1) { 
        perror("Bind error");
    }
    
    if( listen(listen_fd, 5) == -1 ) {
        perror("Failed to listen for incoming connexions");
    }
    
    while(1) {
        int client = accept(listen_fd, NULL, 0);
        if( client == -1 ){
            perror("Failed to accept a call");
        }
        
        int pid = fork();
        
        if(!pid) {
            int number;
            int received = recv(client, &number, sizeof(number), 0);
            if(received == -1) {
               perror("Failed to receive the number"); 
            }
            int squared = number*number;
            int sent = send(client, &squared, sizeof(squared), 0);
            exit(0);
        }
    }
}