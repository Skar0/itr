#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {

int nbr = atoi(argv[1]);
int nbr2 = atoi(argv[1]);
int pid;

for(nbr; nbr>0; nbr--) {	
	pid = fork();
	
	if(pid == 0) {
		printf("Je suis %d et mon père est le %d \n",getpid(), getppid());
		return 0;
	}

}

for(nbr2; nbr2>0;nbr2--){
	wait(NULL);
}
exit(0);
}

