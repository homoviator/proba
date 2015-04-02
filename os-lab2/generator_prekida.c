#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

int pid = 0;

void prekidna_rutina(int sig){
	/* pošalji SIGKILL procesu 'pid'*/
	kill( pid, SIGKILL );
	exit(0);
}

int main(int argc, char *argv[]){
	pid = atoi(argv[1]);
	sigset(SIGINT, prekidna_rutina);
	
	while(1){
	/* odspavaj 3-5 sekundi */
	/* slučajno odaberi jedan signal (od 4) */
	/* pošalji odabrani signal procesu 'pid' funkcijom kill*/
	sleep( rand()%3 + 3 );
	int s = rand()%4 + 1;
	if ( s == 1 ) kill (pid, SIGUSR1);
	if ( s == 2 ) kill (pid, SIGUSR2);
	if ( s == 3 ) kill (pid, SIGTTIN);
	if ( s == 4 ) kill (pid, SIGPIPE); 	
	}
	
	return 0;
}
