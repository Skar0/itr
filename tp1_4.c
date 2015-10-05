#include <stdio.h>
#include <stdlib.h>

int int main(int argc, char const *argv[])
{
	//Création du fd du tube
	int fd[2]; //Descripteur du tube
	pipe(fd); //Tube

	int pid = fork(); // On crée un fils

	if (pid == 0) {


	}

	else if (pid > 0) {


	}

	else {
		exit(-1);
	}

}