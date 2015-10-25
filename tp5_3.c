#include <pthread.h>
#include <stdio.h>
pthread_t thread_ids[99];
char *words_array[99];
    
void printMessage(int j) {
    
    if(words_array[j+1] != NULL) {
        pthread_join(thread_ids[j+1], NULL);
    }
    printf("%s\n", words_array[j]);
}

int main() {

    char message[100];
    scanf ("%[^\n]%*c", message);

    const char separator[2] = " ";

    /*
    Ici, vouloir changer message (ce que strtok fait n'est pas permit car une chaine de caractère n'est pas considérée de base comme tableau de char
    La solution est de travailler sur une copie de la chaine ou sur la chaine transformée en tableau de char
    */
    int i = 0;
    int j = 0;
    int k = 0;
    
    char *copy = strdup(message); // Copie la chaine
    words_array[i] = strtok(copy, separator); 
    
    while(words_array[i] != NULL) {
        i++;
        words_array[i] = strtok(NULL, separator);
    }
    i--;
    while(i>=0) {
        if( (pthread_create(&thread_ids[i], NULL, &printMessage, i ) ) < 0) {
            perror("ERROR CREATING THE THREAD");
        }
        
        pthread_join(thread_ids[i], NULL);
        i--;
    }
    
}