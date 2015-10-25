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
    
    switch(fork()) {
        
        case 0: 
            {
                int sum1 = 0;
                int number1_from_pipe = 1;
                int finished = 0;

                while(!finished) {
                    
                    struct msgbuf msg_son1;
                        msg_son1.mtype = 1;
                        msg_son1.mtext[1] = number1_from_pipe;
                    
                    if(  msgrcv(msgid,&msg_son1,(sizeof(struct msgbuf) - sizeof(long)), 1,0) == -1) {
                        perror("Failed to receive message son 1");
                    }
                    
                    printf("%d\n", msg_son1.mtext[1]);
                    sum1 = sum1 + msg_son1.mtext[1];
                    
                    if(msg_son1.mtext[1] == 0) {
                        
                        struct msgbuf msg_sum1;
                            msg_sum1.mtype = 1;
                            msg_sum1.mtext[1]=sum1;
                        
                        if( msgsnd(msgid,&msg_sum1,(sizeof(struct msgbuf) - sizeof(long)), 0) == -1 ) {
                            perror("Failed to send sum 1");
                        }
                        
                        finished = 1;
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
                                    
                                struct msgbuf msg_son2;
                                msg_son2.mtype = 2;
                                msg_son2.mtext[1]=number2_from_pipe;
                                
                                if( msgrcv(msgid,&msg_son2,(sizeof(struct msgbuf) - sizeof(long)), 2,0) == -1) {
                                    perror("Failed to receive son 2");
                                }
                                    
                                sum2 = sum2 + msg_son2.mtext[1];
                                
                                if(msg_son2.mtext[1] == 0) {
                                    
                                    struct msgbuf msg_sum2;
                                        msg_sum2.mtype = 2;
                                        msg_sum2.mtext[1]=sum2;
                                    
                                    msgsnd(msgid,&msg_sum2,(sizeof(struct msgbuf) - sizeof(long)), 0); 
                                    
                                    finished = 1;
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
                                scanf ("%d",&temp);
                                getchar(); //Pour éviter de re-rentrer dans la boucle avec le charactère de enter
                                
                                //Si temp est + on l'envoie dans le tube et on permet au fils 1 de lire le tube en incr la sem
                                if(temp > 0) {
                                    positive_entered = 1;
                                    
                                    struct msgbuf msg_pos;
                                        msg_pos.mtype = 1;
                                        msg_pos.mtext[1]=temp;
                                    msgsnd(msgid, &msg_pos,(sizeof(struct msgbuf) - sizeof(long)), 0);
                                }
                                
                                if(temp < 0) {
                                    negative_entered = 1;
                                    
                                    struct msgbuf msg_neg;
                                        msg_neg.mtype = 2;
                                        msg_neg.mtext[1]=temp;

                                    msgsnd(msgid,&msg_neg,(sizeof(struct msgbuf) - sizeof(long)), 0); 
                                }
                                
                                if(temp == 0) {
                                    
                                    printf("FINI\n");
                                    
                                    struct msgbuf msg_zero1;
                                        msg_zero1.mtype = 1;
                                        msg_zero1.mtext[1]=temp;
                                    
                                    msgsnd(msgid,&msg_zero1,(sizeof(struct msgbuf) - sizeof(long)), 0); 
                                    
                                    struct msgbuf msg_zero2;
                                        msg_zero2.mtype = 2;
                                        msg_zero2.mtext[1]=temp;
                                    
                                     msgsnd(msgid,&msg_zero2,(sizeof(struct msgbuf) - sizeof(long)), 0); 
                                }
                            }
                            
                            while(temp != 0);
                            
                            if(!positive_entered) {

                            }
                            
                            if(!negative_entered) {

                            }
                            
                            int res1;
                            int res2;
                            
                            struct msgbuf msg_res1;
                                msg_res1.mtype = 0;
                                msg_res1.mtext[1]=res1;
                            
                            msgrcv(msgid,&msg_res1,(sizeof(struct msgbuf) - sizeof(long)), 1,0);
                            
                            struct msgbuf msg_res2;
                                msg_res2.mtype = 0;
                                msg_res2.mtext[1]=res2;
                                
                            msgrcv(msgid,&msg_res2,(sizeof(struct msgbuf) - sizeof(long)), 2,0); 
                            
                            printf("\nSUM 1 = %d\n", msg_res1.mtext[1]);
                            printf("SUM 2 = %d\n", msg_res2.mtext[1]);
                    
                            wait(NULL);
                            wait(NULL);
                        }
                }
        
            
            }
            
    }
    

}