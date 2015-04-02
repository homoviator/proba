#include <stdio.h>
#include <signal.h>
#include <unistd.h> // zbog fje sleep()

#define N 6    /* broj razina prekida */

int OZNAKA_CEKANJA[N]={0,0,0,0,0,0};

int PRIORITET[N]={0,0,0,0,0,0};

int TEKUCI_PRIORITET = 0;

int sig[]={SIGUSR1, SIGUSR2, SIGPIPE, SIGTTIN, SIGINT};

void zabrani_prekidanje(){
	int i;

	for(i=0; i<5; i++)
		sighold(sig[i]);
}

void dozvoli_prekidanje(){
	int i;

	for(i=0; i<5; i++)
		sigrelse(sig[i]);
}

void obrada_prekida(int i){
   /* obrada se simulira trošenjem vremena,
      obrada traje 5 sekundi, ispis treba biti svake sekunde */
	int j, k, x;
	for( k = 0; k < N; ++k ){
		if( k == i )printf( "P " );
		else printf( "- " );
	}
	
	printf( " O_CEK[" );
	for( x = 0; x < N; ++x )
		printf( "%d ", OZNAKA_CEKANJA[x] );
	printf( "]" );
	printf( " TEK_PRIOR=%d ", TEKUCI_PRIORITET );
	printf( " PRIOR[" );
	for( x = 0; x < N; ++x )
		printf( "%d ", PRIORITET[x] );
	printf( "]" );
	printf( "\n" );
	for( j = 0; j < 5; ++j ){
		sleep( 1 );
		for( k = 0; k < N; ++k ){
			if( k == i ) printf( "%d ", j + 1 );
			else printf( "- " );
		}
	
	printf( " O_CEK[" );
	for( x = 0; x < N; ++x )
	printf( "%d ", OZNAKA_CEKANJA[x] );
	printf( "]" );
	printf( " TEK_PRIOR=%d ", TEKUCI_PRIORITET );
	printf( " PRIOR[" );
	for( x = 0; x < N; ++x )
		printf( "%d ", PRIORITET[x] );
	printf( "] " );

	printf( "\n" );
	}
	for( k = 0; k < N; ++k ){
		if( k == i ) printf( "K " );
		else printf( "- " );
	}
	
	printf( " O_CEK[" );
	for( x = 0; x < N; ++x )
		printf( "%d ", OZNAKA_CEKANJA[x] );
	printf( "]" );
	printf( " TEK_PRIOR=%d ", TEKUCI_PRIORITET );
	printf( " PRIOR[" );
	for( x = 0; x < N; ++x )
		printf( "%d ", PRIORITET[x] );
	printf( "]" );
	
	printf( "\n" );
	return;
}

void prekidna_rutina(int sig){
	int n=1;
	
	zabrani_prekidanje();
	switch(sig){
     case SIGUSR1: 
		n=1; 
          printf("- X - - - -\n");
          break;

	case SIGUSR2: 
		n=2; 
          printf("- - X - - -\n");
          break;

	case SIGPIPE:
		n=3;
		printf("- - - X - -\n");
		break;

	case SIGTTIN:
		n=4;
		printf("- - - - X -\n");
		break;

	case SIGINT:
		n=5;
		printf("- - - - - X\n");
		break;
	 
	}
	int x;
	OZNAKA_CEKANJA[n] += 1;
	do{
	x = 0;
	int j;
	for ( j = TEKUCI_PRIORITET + 1; j < N; ++j) {
		if ( OZNAKA_CEKANJA[j] != 0 ) x = j; 
	 }
	 if ( x > 0 ) {
		OZNAKA_CEKANJA[x] -= 1;
		PRIORITET[x] = TEKUCI_PRIORITET;
		TEKUCI_PRIORITET = x;

          dozvoli_prekidanje();

          obrada_prekida(x);

          zabrani_prekidanje();

          TEKUCI_PRIORITET = PRIORITET[x];
	  }
	} while( x > 0 );

	dozvoli_prekidanje();

}

int main ( void ){

	sigset (SIGUSR1, prekidna_rutina);
	sigset (SIGUSR2, prekidna_rutina);
	sigset (SIGPIPE, prekidna_rutina);
	sigset (SIGTTIN, prekidna_rutina);
	sigset (SIGINT, prekidna_rutina);
   
     printf("Proces obrade prekida, PID=%d\n", getpid());

   /* troši vrijeme da se ima šta prekinuti - 10 s */

	int i, x;
	printf( "GP S1 S2 S3 S4 S5\n" );
	printf( "-----------------\n" );
	for( i = 0; i < 10; ++i ){
		printf( "%2d - - - - - ", i + 1 );
		
		printf( " O_CEK[" );
		for( x = 0; x < N; ++x )
			printf( "%d ", OZNAKA_CEKANJA[x] );
		printf( "]" );
		printf( " TEK_PRIOR=%d ", TEKUCI_PRIORITET );
		printf( " PRIOR[" );
		for( x = 0; x < N; ++x )
			printf( "%d ", PRIORITET[x] );
		printf( "]" );
		
		printf( "\n" );
		sleep( 1 );
	}
   printf ("Zavrsio osnovni program\n");

   return 0;
}
