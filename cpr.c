/*------------------------------------------------------------
Fichier: cpr.c

Nom: Amin Dhouib
Numero d'etudiant: 300025259

Description: Ce programme contient le code pour la creation
             d'un processus enfant et y attacher un tuyau.
	     L'enfant envoyera des messages par le tuyau
	     qui seront ensuite envoyes a la sortie standard.

Explication du processus zombie
(point 5 de "A completer" dans le devoir):

Juste après la terminaison d'un processus, on peut voir que son entrée 
existe toujours dans le tableau des processus (obtenu à partir de la commande ps -u)
avec <defunct> écrit à côté, ceci veut dire que ledit processus est en effet terminé,
mais on a besoin de temporairement garder son entrée afin que le parent puisse lire le signal de sorti
(exit status) de cet enfant pour savoir que celui-ci est terminé. Ceci est l'état zombie; qui veut
tout simplement dire que l'entrée d'un processus enfant demeure temporairement dans le tableau de processus
après la terminaison de ce dernier pour permettre au parent de lire son signal de sorti, une fois ceci fait,
l'entrée de ce processus est définitivement enlevée.  

-------------------------------------------------------------*/
#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

/* Prototype */
void creerEnfantEtLire(int );

/*-------------------------------------------------------------
Function: main
Arguments: 
	int ac	- nombre d'arguments de la commande
	char **av - tableau de pointeurs aux arguments de commande
Description:
	Extrait le nombre de processus a creer de la ligne de
	commande. Si une erreur a lieu, le processus termine.
	Appel creerEnfantEtLire pour creer un enfant, et lire
	les donnees de l'enfant.
-------------------------------------------------------------*/

int main(int ac, char **av)
{
    int numeroProcessus; 

    if(ac == 2)
    {
       if(sscanf(av[1],"%d",&numeroProcessus) == 1)
       {
           creerEnfantEtLire(numeroProcessus);
       }
       else fprintf(stderr,"Ne peut pas traduire argument\n");
    }
    else fprintf(stderr,"Arguments pas valide\n");
    return(0);
}


/*-------------------------------------------------------------
Function: creerEnfantEtLire
Arguments: 
	int prcNum - le numero de processus
Description:
	Cree l'enfant, en y passant prcNum-1. Utilise prcNum
	comme identificateur de ce processus. Aussi, lit les
	messages du bout de lecture du tuyau et l'envoie a 
	la sortie standard (df 1). Lorsqu'aucune donnee peut
	etre lue du tuyau, termine.
-------------------------------------------------------------*/

void creerEnfantEtLire(int prcNum)
{	
	char prcNumStr[10];
	char readBuffer[128];
	int nextPrcNum, nbytes;
	nbytes = -1;
	nextPrcNum = prcNum - 1;

	/*Signal process is starting */
	printf("Processus %d commence\n",prcNum);
	fflush(stdout);

	if (prcNum > 1) {
		/*For processes n to 2*/

		/*Create pipe to communicate with child */
		/*Create fd table and pipe */
		int fd[2];

		if(pipe(fd)== -1){
			fprintf(stderr, "Tuyau mal construit.\n");
		}

		/*Create child process */
		pid_t cpid = fork();

		if (cpid < 0) { /*Error */
			fprintf(stderr,"Could not create process\n");
			exit(-1);
		} else if (cpid == 0) { /*Child process */
			close(fd[0]); /*Close reading side of the pipe */
			dup2(fd[1],1); /*Redirect stdout to pipe writing end */
			sprintf(prcNumStr,"%d",nextPrcNum); /*Convert num to string */
			execl("./cpr","./cpr",prcNumStr, NULL); /*Execute cpr num-1 */
			exit(-1);

		} else { /*Parent process */
			close(fd[1]); /*Close writing end of the pipe */
			while (nbytes != 0) { /*While pipe is still open */
				nbytes = read(fd[0],readBuffer,sizeof(readBuffer)); /*Read from pipe */
				printf("%s",readBuffer);
				fflush(stdout);
				memset(readBuffer,0,sizeof(readBuffer)); /*Clear string */
			}
			printf("Processus %d termine\n",prcNum);
			fflush(stdout);
			sleep(10);
		}

	} else {
		/*This is the last process - wait for 5 seconds before terminating*/
		sleep(5);
		printf("Processus %d termine\n",prcNum);
		fflush(stdout);
	}
}
