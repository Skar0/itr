#include <stdio.h>
#include <stdlib.h>

int main() {

printf("Je suis le processus %d et mon père est le processus %d \n", getpid(), getppid());  

int pid1 = fork();

if(pid1 == 0) {

	printf("Je suis le processus %d et mon père est le processus %d \n", getpid(), getppid());
	int pid3 = fork();
	
	if(pid3 == 0) {
		printf("Je suis le processus %d et mon père est le processus %d \n", getpid(), getppid());  
	}
	else if(pid3<0) {
		exit(-1);
	}
	wait(NULL);
}

if(pid1 > 0) {

	int pid2 = fork();
	
	if(pid2 == 0) {

		printf("Je suis le processus %d et mon père est le processus %d \n", getpid(), getppid());  
	}
	else if(pid2<0) {
		exit(-1);
	}  
	wait(NULL);
}	

else {
	exit(-1);
}
	wait(NULL);
	exit(0);
}
