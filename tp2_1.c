#include <stdio.h>
#include <stdlib.h>
#include <signal.h>


void affichage_recursif() {

	printf("Après 1sec\n");
	sleep(1);
	affichage_recursif();

}

void SIGALRM_HANDLER() {
	printf("Après 5sec\n");
	alarm(5);

}

int main() {

struct sigaction sa_SIGINT;
sa_SIGINT.sa_handler = SIG_IGN;
sigfillset(&sa_SIGINT.sa_mask);
sa_SIGINT.sa_flags=0;
sigaction(SIGINT,&sa_SIGINT,NULL);

struct sigaction sa_SIGARLRM;
sa_SIGARLRM.sa_handler = *SIGALRM_HANDLER; //C'est ma fonction qui est appelée pour handle le signal
sigfillset(&sa_SIGARLRM.sa_mask);
sa_SIGARLRM.sa_flags=0; //Toujours mettre 0
sigaction(SIGALRM,&sa_SIGARLRM,NULL); //Permet de lier le signal à son handler

alarm(5);
affichage_recursif();


}

