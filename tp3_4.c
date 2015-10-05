#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>


int main() {
    
    int msgid = msgget(IPC_PRIVATE, 0660);
    
    struct msgbuf {
        long mtype;
        char mtext[1];
    };
    
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
                    
                    struct msgbuf msg_son1;
                    msg_son1.mtype = 0;
                    msg_son1.mtext[1] = number1_from_pipe;
                    
                    if(  msgrcv(msgid,&msg_son1,sizeof(msg_son1), 0,0) == -1) {
                        perror("Failed to receive message son 1");
                    }
                    
                    sum1 = sum1 + number1_from_pipe;
                    
                    if(number1_from_pipe == 0) {
                        
                        struct msgbuf msg_sum1;
                        msg_sum1.mtype = 0;
                        msg_sum1.mtext[1]=sum1;
                        
                        if( msgsnd(msgid,&msg_sum1,sizeof(msg_sum1), 0) == -1 ) {
                            perror("Failed to send sum 1");
                        }
                        
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
                                    
                                struct msgbuf msg_son2;
                                msg_son2.mtype = 0;
                                msg_son2.mtext[1]=number2_from_pipe;
                                
                                if( msgrcv(msgid,&msg_son2,sizeof(msg_son2), 0,0) == -1) {
                                    perror("Failed to receive son 2");
                                }
                                    
                                sum2 = sum2 + number2_from_pipe;
                                
                                if(number2_from_pipe == 0) {
                                    
                                    struct msgbuf msg_sum2;
                                    msg_sum2.mtype = 0;
                                    msg_sum2.mtext[1]=sum2;
                                    
                                    msgsnd(msgid,&msg_sum2,sizeof(msg_sum2), 0); 
                                    
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
                                    
                                    struct msgbuf msg_pos;
                                    msg_pos.mtype = 0;
                                    msg_pos.mtext[1]=temp;

                                    msgsnd(msgid, &msg_pos,sizeof(msg_pos), 0); 
                                    
                                    //On incremente la sem1 pour permettre de continuer
                                    if( semop(semaphore, &sem1_op, 1) == -1 ) {
                                            perror("ERROR INCREMENTING THE SEMAPHORE");
                                    }
                                }
                                
                                if(temp < 0) {
                                    negative_entered = 1;
                                    
                                    struct msgbuf msg_neg;
                                    msg_neg.mtype = 0;
                                    msg_neg.mtext[1]=temp;

                                    msgsnd(msgid,&msg_neg,sizeof(msg_neg), 0); 
                                    
                                    //On incremente la sem2 pour permettre de continuer
                                    if( semop(semaphore, &sem2_op, 1) == -1 ) {
                                            perror("ERROR INCREMENTING THE SEMAPHORE");
                                    }
                                }
                                
                                if(temp == 0) {
                                    struct msgbuf msg_zero1;
                                    msg_zero1.mtype = 0;
                                    msg_zero1.mtext[1]=temp;
                                    
                                    msgsnd(msgid,&msg_zero1,sizeof(msg_zero1), 0); 
                                     
                                    if( semop(semaphore, &sem1_op, 1) == -1 ) {
                                        perror("ERROR INCREMENTING THE SEMAPHORE");
                                    }
                                    
                                    struct msgbuf msg_zero2;
                                    msg_zero2.mtype = 0;
                                    msg_zero2.mtext[1]=temp;
                                    
                                     msgsnd(msgid,&msg_zero2,sizeof(msg_zero2), 0); 
                                     
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
                            
                            struct msgbuf msg_res1;
                            msg_res1.mtype = 0;
                            msg_res1.mtext[1]=res1;
                            
                            msgrcv(msgid,&msg_res1,sizeof(msg_res1), 0,0);
                        
                             if( semop(semaphore, &sem2_prob, 1) == -1 ) {
                                perror("ERROR INCREMENTING THE SEMAPHORE");
                            }
                            
                            struct msgbuf msg_res2;
                            msg_res2.mtype = 0;
                            msg_res2.mtext[1]=res2;
                            msgrcv(msgid,&msg_res2,sizeof(msg_res2), 0,0); 
                            printf("\nSUM 1 = %d\n", res1);
                            printf("SUM 2 = %d\n", res2);
                    
                            wait(NULL);
                            wait(NULL);
                        }
                }
        
            
            }
            
    }
    

}