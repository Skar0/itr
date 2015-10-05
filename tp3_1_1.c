#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
int main() {

	int nbr1 = 0;
	int nbr2 = 0;

	int semaphore;
	if( (semaphore = semget(IPC_PRIVATE, 1, 0660)) == -1 ) {
		perror("ERROR CREATING THE SEMAPHORE");
	}
	//Private permet de limiter la semahore au père et ses descendants

	/*
	struct sembuf sbuf;
		sbuf.sem_num = 0;
		sbuf.sem_op = 1;
		sbuf.sem_flg = 0;
		semop(semaphore, &sbuf, 1);
	*/	
	if ( semctl(semaphore, 0, SETVAL, 1) == -1) {
		perror("ERROR SETTING SEMAPHORE VALUE TO 1");
	}
	struct sembuf sem_op;
			sem_op.sem_num = 0;
			sem_op.sem_op = 1;
			sem_op.sem_flg = 0;

	struct sembuf sem_prob;
			sem_prob.sem_num = 0;
			sem_prob.sem_op = -1;
			sem_prob.sem_flg = 0;
	
	switch(fork()) {

	case 0 : {
			//filio due

			if( semop(semaphore, &sem_prob, 1) == -1 ) {
				perror("ERROR DECREMENTING THE SEMAPHORE");
			}
			
			for(nbr1; nbr1<5; nbr1++) {
				printf("JE SUIS LE FILS DE PID %d \n", getpid());
				sleep(1);
			}

			if( semop(semaphore, &sem_op, 1) == -1 ) {
				perror("ERROR INREMENTING THE SEMAPHORE");
			}
			break;

		}
	
	case -1 : {
			//Error
			perror("ERROR CREATING A SON PROCESS");
			return -1;
		}

	default : {
			//Padre
			
			switch(fork()) {
			
			case 0 : {
					//Filio uno
					

					if( semop(semaphore, &sem_prob, 1) == -1 ) {
						perror("ERROR DECREMENTING THE SEMAPHORE");
					}
					for(nbr2; nbr2<5; nbr2++) {
						printf("JE SUIS LE FILS DE PID %d \n", getpid());
						sleep(1);
					}
					if( semop(semaphore, &sem_op, 1) == -1) {
						perror("ERROR DECREMENTING THE SEMAPHORE");
					} 
					break;
				}
			case -1 : {
					//Error
					perror("ERROR CREATING A SON PROCESS");
					return -1;
				}
			default : {
					//Padre
					wait(NULL);
					wait(NULL);
					if( semctl(semaphore, 0, IPC_RMID, NULL) == -1 ){
						perror("ERROR LIBERATING SEMAPHORE");
					}
					break;
				}
			} 
		break;
		}
	}
	exit(0);
}


