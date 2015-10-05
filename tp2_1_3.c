#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main() {

//Structure donnée en param de signaction
struct sigaction sa;
//On remplit la structure
sa.sa_handler = SIG_IGN; //SIG IGN permet d'ignorer le signal
sigfillset(&sa.sa_mask); //Donne l'ensemble de signaux ignorés pendant l'ensemble de la fonction
sa.sa_flags=0; //Tjrs à 0 !!!

sigaction(SIGINT,&sa,NULL);

affichage_recursif();


}

affichage_recursif() {

	printf("Salut c'est cool\n");
	affichage_recursif();

}
