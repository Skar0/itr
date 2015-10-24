#include <stdio.h>
#include <pthread.h>

int matrice[10][4]=	{8, 25, 3, 41, 
			11, 18, 3, 4,
			4, 15, 78, 12,
			1, 12, 0, 12, 
			7, 9, 13, 15,
			4, 21, 37, 89,
			1, 54, 7, 3, 
			15, 78, 7, 1,
			12, 15, 13, 47,
			91, 13, 72, 90};
                    
int vecteur[4]=		{1, 2, 3, 4};

int resultat[10] = {0,0,0,0,0,0,0,0,0,0}; //c'est une col résultat
pthread_mutex_t mutex;
void computeVector(int arg) {
    
    printf("\n SALUT ------\n");
    int i = 0;
    while(i<10) {
        pthread_mutex_lock(&mutex);
        resultat[i] = resultat[i] + matrice[i][arg]*vecteur[arg];
        pthread_mutex_unlock(&mutex);
        i++;
    }
    /*
     i=0;
    while(i<10) {
        printf("\n%d", resultat[i]);
        i++;
    }
    */
}

int main() {
   if( pthread_mutex_init(&mutex, NULL) < 0) {
    perror("ERROR INITIALISING MUTEX");
   }
    int i = 0;
    pthread_t thread_ids[4];
    while (i<4) {
        if( pthread_create(&thread_ids[i], NULL, &computeVector, i) < 0) {
                perror("ERROR CREATING THE THREAD");
        }
        i++;
    }

    i =0;
    while( i < 4) {
        if( pthread_join(thread_ids[i], NULL) < 0 ) {
            perror("ERROR JOINING");
        }
        i++;
    }
   // sleep(5);
    i=0;
    while(i<10) {
        printf("\n%d", resultat[i]);
        i++;
    }
}