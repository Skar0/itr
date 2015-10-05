#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <ctype.h>
#include <signal.h>
int semaphore;
int shmid;

    void SIGINT_HANDLER() {
        semctl(semaphore, 0, IPC_RMID, NULL); 
        semctl(semaphore, 1, IPC_RMID, NULL);
        semctl(semaphore, 2, IPC_RMID, NULL);
        shmctl(shmid,IPC_RMID, NULL);
        exit(0);
    }

int main() {
    
    
    //Quand il y a un identifiant, plusieur ressources appellent la même sem du coup on doit mentionner qui l'a crée via le IPC_CREATE
    if( (semaphore = semget(42421, 3, 0666|IPC_CREAT)) == -1 ) {
        perror("ERROR CREATING THE SEMAPHORE");
    }
    
    if ( semctl(semaphore, 0, SETVAL, 1) == -1) {
        perror("ERROR SETTING SEMAPHORE 1 VALUE TO 1");
    }
    
    if ( semctl(semaphore, 1, SETVAL, 0) == -1) {
        perror("ERROR SETTING SEMAPHORE 2 VALUE TO 1");
    }
    
    if ( semctl(semaphore, 2, SETVAL, 0) == -1) {
        perror("ERROR SETTING SEMAPHORE 3 VALUE TO 1");
    }
    
    //On s'occupe de supprimer les semaphores au ctrl-c
    struct sigaction sa_SIGINT;
    sa_SIGINT.sa_handler = *SIGINT_HANDLER;
    sigfillset(&sa_SIGINT.sa_mask);
    sa_SIGINT.sa_flags=0;
    sigaction(SIGINT,&sa_SIGINT,NULL);

    
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
        
    if( (shmid = shmget(42422, sizeof(char)*10, 0666|IPC_CREAT) ) < 0) {
        perror("FAILED TO RESERVE SHARED MEMORY");
    }
    
    char * reader;
    if( (reader = (char *) shmat(shmid, NULL, SHM_R|SHM_W) ) < 0) {
        perror("FAILED TO ATTACH SHARED MEMORY ON USER SPACE");
    }

    char buffer[10];
    int i = 0;
    //Boucle infinie pour attendre un input
    while(1) {
        
        //Diminuer la semaphore qui sert à arrêter le serveur à un client a la fois
        if( semop(semaphore, &sem3_prob, 1) == -1 ) {
            perror("ERROR DECREMENTING THE SEMAPHORE");
        }
        
        
        while(i<10 && reader[i] != '\0') {
            reader[i] = reader[i]-32;
            i++;
            
        }
        i = 0;
        //Augmenter la semaphore qui dit qu'on a fini l'opération et que le client peut lire le résultat
        if( semop(semaphore, &sem2_op, 1) == -1 ) {
            perror("ERROR INCREMENTING THE SEMAPHORE");
        }

        
    }
    
}