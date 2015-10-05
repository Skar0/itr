#include <stdio.h>
#include <stdlib.h>

int int main(int argc, char const *argv[])
{

	//On crée les pipes avant de fork pour que ceux ci soient partagés

	int fd1[2]; //Descripteur du tube du fils 1
	int fd2[2]; //Descripteur du tube du fils 2

	pipe(fd1); //Tube 1
	pipe(fd2); //Tube 2

	int pid1 = fork(); // On crée un fils

	if(pid > 0) {

		//On est dans le père et on recrée un fils

		int pid2 = fork();

		if(pid2>0) {
			//On est dans le père
		}

		else if (pid2 ==0 ) {
			int p1;
			int sum1;
			do {
				read(fd1[0],&p1,sizeof(int));
				p1 += sum1;
			}
			while (p1 != 0);

			//ENVOIE LE RESULTAT ET TUE LE PROCESS

			write(fd1[1],&sum1,sizeof(int));


		}

		else {	
			//Ca a foiré
			exit(-1);
		}

		//Après la création des fils, on scanne

		int input = 1;

		do {
			printf("Enter a number (0  ends the input)");
    		scanf( "%d", &input);

    		if(input>0) {
    			//Si positif
    			write(fd1[1],&input,sizeof(int));
    		}

    		if(input<0) {
    			//Si négatif
    			write(fd2[1],&input,sizeof(int));
    		}
		}
		while (input !=0);

		wait(NULL);
		wait(NULL);

		int res1;
		int res2;

		read(fd2[0],&res2,sizeof(int));
		read(fd1[0],&res1,sizeof(int));

		printf("%s %s\n", res1, res2);
	}

	else if (pid1 == 0 ) {

		int p2;
		int sum2;
		do {
			read(fd2[0],&p2,sizeof(int));
			p2 += sum2;
		}
		while (p2 != 0);

		//ENVOIE LE RES ET TUE LE PROCESS

		write(fd2[1],&sum2,sizeof(int));

	}

	else {
		//Ca a foiré
		exit(-1);
	}
}