#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main() {
    
    //Creation du socket
    int read_fd = socket(PF_INET, SOCK_STREAM, 0); //Creation d'un socket IP (PF_INET) + TCP (le flag 0) et mise de son descripteur de fichier dans read_fd
    
    if (read_fd == -1) {
       perror("Failed to create socket");
    } 
    
    //Adresse IPV4 d'écoute
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(4237);
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //Récupère l'adresse locale
     
    int hostname = 4;
    
    if (connect(read_fd, (const struct sockaddr*) &addr, sizeof(addr)) == -1 ) {
        perror("Failed to connect to server");
    }
    
    if(send(read_fd, &hostname, sizeof(hostname), 0) == -1) {
        perror("Failed to send message");
    }
    int number;
    int received = recv(read_fd, &number, sizeof(number), 0);
    if(received == -1) {
        perror("Failed to receive the number"); 
    }
    
    printf("\n%d\n", number);
}