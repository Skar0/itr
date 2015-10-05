#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int pid;

void SIGUSR1_HANDLER() {
	printf("Je tue mon fils");
	kill(pid, SIGKILL);
}

int main() {

pid = fork();

switch(pid) {

	case 0:	
		{
			char c1 = 't';
			while(c1 != 'y')
			{
				printf("Fils ­: Envoyer un signal ? (y/n)\n");
				scanf("%c", &c1);
				getchar();
			}

			kill (getppid(), SIGUSR1);
			printf("Je meurs");
			pause();
		}
	break;

	case -1:
	break;

	default :
		{
		struct sigaction sa_SIGUSR1;
		sa_SIGUSR1.sa_handler = SIGUSR1_HANDLER; //C'est ma fonction qui est appelée pour handle le signal
		sigfillset(&sa_SIGUSR1.sa_mask);
		sa_SIGUSR1.sa_flags=0; //Toujours mettre 0
		sigaction(SIGUSR1,&sa_SIGUSR1,NULL); //Permet de lier le signal à son handler
		wait();
		}
	break;



}
	
}

