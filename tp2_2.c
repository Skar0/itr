#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

float compteur = 5;
struct itimerval timer;

void SIGALRM_HANDLER() {
	
	compteur -= 0.1f;
	printf("%f\n", compteur);

	if(compteur < 0) {
		exit(0);
	}
}

int main() {


	struct sigaction sa_SIGARLRM;
	sa_SIGARLRM.sa_handler = *SIGALRM_HANDLER; //C'est ma fonction qui est appelée pour handle le signal
	sigfillset(&sa_SIGARLRM.sa_mask);
	sa_SIGARLRM.sa_flags=0; //Toujours mettre 0
	sigaction(SIGALRM,&sa_SIGARLRM,NULL); //Permet de lier le signal à son handler

	struct timeval temps;
	temps.tv_sec = 0;
	temps.tv_usec = 100000;

	struct timeval temps2;
	temps2.tv_sec = 0;
	temps2.tv_usec = 100000;

	timer.it_interval = temps;
	timer.it_value = temps2;

	setitimer(ITIMER_REAL, &timer, NULL);

	while(1) {
		pause();
	}
}


