#include <pthread.h>
#include <stdio.h>

int main() {

char message[100] = "salut c'est cool \0";
//fgets (message, 99, stdin);
const char s[2] = " ";
char *token;
token = strtok(message, s);
int thread_ids[99];
int i = 0;

while(token != NULL) {
    
    printf( " %s\n", token );
    
    token = strtok(NULL, s);
    /*
    if( ( thread_ids[i] = pthread_create(NULL, NULL, printMessage, &token ) ) < 0) {
        perror("ERROR CREATING THE THREAD");
    }
    */
}

/*
void *printMessage( void *token) {
    printf(token);
}
*/
}