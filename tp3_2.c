#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main() {
    
    int fd[2]; //Descripteur de fichier du tube
    pipe(fd);
    
    int semaphore;
    if( (semaphore = semget(IPC_PRIVATE, 2, 0660)) == -1 ) {
        perror("ERROR CREATING THE SEMAPHORE");
    }
    //Private permet de limiter la semahore au père et ses descendants
    
    if ( semctl(semaphore, 0, SETVAL, 0) == -1) {
        perror("ERROR SETTING SEMAPHORE 1 VALUE TO 1");
    }
    
    if ( semctl(semaphore, 1, SETVAL, 0) == -1) {
        perror("ERROR SETTING SEMAPHORE 2 VALUE TO 1");
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
    
    switch(fork()) {
        
        case 0: 
            {
                int sum1 = 0;
                int number1_from_pipe = 1;
                int finished = 0;

                while(!finished) {

                    //On decremente la sem (tente d'accéder à la ressource)
                    if( semop(semaphore, &sem1_prob, 1) == -1 ) {
                        perror("ERROR DECREMENTING THE SEMAPHORE");
                    }
                    
                    read(fd[0],&number1_from_pipe,sizeof(int));
                    
                    sum1 = sum1 + number1_from_pipe;
                    
                    if(number1_from_pipe == 0) {
                         write(fd[1],&sum1,sizeof(int));
                         finished = 1;
                        if( semop(semaphore, &sem1_op, 1) == -1 ) {
                            perror("ERROR INCREMENTING THE SEMAPHORE");
                        }
                    }
                    
                }
                
                                   
                
                break;
            }
        
        case -1:
            {
                printf("ERROR WITH FORK");
                return -1;
                break;
            }
            
        default:
            {
                            
                switch(fork()) {
                                
                    case 0:
                        {
                            int sum2 = 0;
                            int number2_from_pipe = 1;
                            int finished = 0;
                            
                            while(!finished) {

                                //On decremente la sem (tente d'accéder à la ressource)
                                if( semop(semaphore, &sem2_prob, 1) == -1 ) {
                                    perror("ERROR DECREMENTING THE SEMAPHORE");
                                }
                                    
                                read(fd[0],&number2_from_pipe,sizeof(int));
                                
                                sum2 = sum2 + number2_from_pipe;
                                
                                if(number2_from_pipe == 0) {
                                    write(fd[1],&sum2,sizeof(int));
                                    finished = 1;
                                    if( semop(semaphore, &sem2_op, 1) == -1 ) {
                                        perror("ERROR INCREMENTING THE SEMAPHORE");
                                    }
                                }
                    
                            } 
                            
                            break;
                        }
                            
                    case -1:
                        {
                        printf("ERROR WITH FORK");
                        return -1;
                        break;
                        }
                        
                    default:
                        {
                        
                            int temp = 1;
                            int negative_entered = 0;
                            int positive_entered = 0;
                
                            do {
                                printf("\nInput an integer (0 triggers the computation)\n");
                               //temp = getchar();
                                scanf ("%d",&temp);
                                getchar(); //Pour éviter de re-rentrer dans la boucle avec le charactère de enter
                                
                                //Si temp est + on l'envoie dans le tube et on permet au fils 1 de lire le tube en incr la sem
                                if(temp > 0) {
                                    positive_entered = 1;
                                    write(fd[1],&temp,sizeof(int)); //On écrit dans le tube
                                    
                                    //On incremente la sem1 pour permettre de continuer
                                    if( semop(semaphore, &sem1_op, 1) == -1 ) {
                                            perror("ERROR INCREMENTING THE SEMAPHORE");
                                    }
                                }
                                
                                if(temp < 0) {
                                    negative_entered = 1;
                                    write(fd[1],&temp,sizeof(int)); //On écrit dans le tube
                                    
                                    //On incremente la sem2 pour permettre de continuer
                                    if( semop(semaphore, &sem2_op, 1) == -1 ) {
                                            perror("ERROR INCREMENTING THE SEMAPHORE");
                                    }
                                }
                                
                                if(temp == 0) {
                                    write(fd[1],&temp,sizeof(int));
                                    if( semop(semaphore, &sem1_op, 1) == -1 ) {
                                        perror("ERROR INCREMENTING THE SEMAPHORE");
                                    }
                                    
                                    write(fd[1],&temp,sizeof(int));
                                    if( semop(semaphore, &sem2_op, 1) == -1 ) {
                                        perror("ERROR INCREMENTING THE SEMAPHORE");
                                    }
                                    
                                }
                            }
                            while(temp != 0);
                            
                            if(!positive_entered) {
                                if( semop(semaphore, &sem1_op, 1) == -1 ) {
                                    perror("ERROR INCREMENTING THE SEMAPHORE");
                                }
                            }
                            
                            if(!negative_entered) {
                                if( semop(semaphore, &sem2_op, 1) == -1 ) {
                                    perror("ERROR INCREMENTING THE SEMAPHORE");
                                }
                            }
                            
                            int res1;
                            int res2;
                            
                            if( semop(semaphore, &sem1_prob, 1) == -1 ) {
                                perror("ERROR INCREMENTING THE SEMAPHORE");
                            }
                            
                            read(fd[0],&res1,sizeof(int));
                            
                             if( semop(semaphore, &sem2_prob, 1) == -1 ) {
                                perror("ERROR INCREMENTING THE SEMAPHORE");
                            }
                            
                            read(fd[0],&res2,sizeof(int));
                            
                            printf("\nSUM 1 = %d\n", res1);
                            printf("SUM 2 = %d\n", res2);
                    
                            wait(NULL);
                            wait(NULL);
                        }
                }
        
            
            }
            
    }
    

}