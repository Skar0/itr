#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <ctype.h>

int main() {
    
    int semaphore;
    if( (semaphore = semget(42421, 3, 0666|IPC_CREAT)) == -1 ) {
        perror("ERROR CREATING THE SEMAPHORE");
    }
    
    int shmid;
    if( (shmid = shmget(42422, sizeof(char)*10, 0666) ) < 0) {
        perror("FAILED TO RESERVE SHARED MEMORY");
    }
    
    char * writers;
    if( (writers = (char *) shmat(shmid, NULL, SHM_R|SHM_W) ) < 0) {
        perror("FAILED TO ATTACH SHARED MEMORY ON USER SPACE");
    }
    
    struct sembuf sem1_op;
        sem1_op.sem_num = 0;
        sem1_op.sem_op = 1;
        sem1_op.sem_flg = 0;

    struct sembuf sem1_prob;
        sem1_prob.sem_num = 0;
        sem1_prob.sem_op = -1;
        sem1_prob.sem_flg = 0;
        
    struct sembuf sem2_op;
        sem2_op.sem_num = 1;
        sem2_op.sem_op = 1;
        sem2_op.sem_flg = 0;

    struct sembuf sem2_prob;
        sem2_prob.sem_num = 1;
        sem2_prob.sem_op = -1;
        sem2_prob.sem_flg = 0; 
        
    struct sembuf sem3_op;
        sem3_op.sem_num = 2;
        sem3_op.sem_op = 1;
        sem3_op.sem_flg = 0;

    struct sembuf sem3_prob;
        sem3_prob.sem_num = 2;
        sem3_prob.sem_op = -1;
        sem3_prob.sem_flg = 0;
       
        
     //Augmenter la semaphore qui sert à permetre au serveur de traiter un client
    if( semop(semaphore, &sem1_prob, 1) == -1 ) {
        perror("ERROR DECREMENTING THE SEMAPHORE");
    }
    
    //writers = "abcdefghij";
    char buffer[12];
    fgets (buffer, 12, stdin);
    strtok(buffer, "\n");
    sprintf(writers, "%s", buffer);
    
    //Diminuer la semaphore
    if( semop(semaphore, &sem3_op, 1) == -1 ) {
        perror("ERROR INCREMENTING THE SEMAPHORE");
    }
    
    //Diminuer la semaphore
    if( semop(semaphore, &sem2_prob, 1) == -1 ) {
        perror("ERROR INCREMENTING THE SEMAPHORE");
    }
    
    
    int i = 0;
    while(i<10 && writers[i] != '\0') {
        printf("\n");
        printf("%c", writers[i]);
        i++;
    }
    printf("\n");
    
     //Diminuer la semaphore
    if( semop(semaphore, &sem1_op, 1) == -1 ) {
        perror("ERROR INCREMENTING THE SEMAPHORE");
    }
    
    exit(0);
    /* POURQUOI writers = "abcde" marceh seulement ici dans le fils
     * donc c'est écrit et lu mais le serveur lui ne voit rien dans la mémoire
     * si je change par sprintf ca marche
     * l'assignation est mauvaise ?
    */
    
}
