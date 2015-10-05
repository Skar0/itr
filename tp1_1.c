#include <stdio.h>
#include <stdlib.h>
int main() {

	int pid = fork();
	int nbr1 = 0;
	int nbr2 = 0;

	switch(fork()) {

            case 0 : {
                //fils 1
                for(nbr1; nbr1<5; nbr1++) {
                    printf("JE SUIS LE FILS DE PID %s \n", getpid());
                    sleep(1);
                }
            }
            
            case -1 : {
                //Error
            }

            default : {
                //Padre
                            
                switch(fork()) {
                            
                    case 0 : {
                        //Fils 2
                        for(nbr2; nbr2<5; nbr2++) {
                            printf("JE SUIS LE FILS DE PID %s \n", getpid());
                            sleep(1);
                        }
                    }
                    
                    case -1 : {
                        //Error
                    }
                    
                    default : {
                        wait(NULL);
                        wait(NULL);
                    }
                } 
            }
	}
}


