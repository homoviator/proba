#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

int pauza=0;
unsigned long broj=1000000001;
unsigned long zadnji = 1000000001;

void periodicki_ispis () 
{
   printf("zadnji prosti broj: %lu\n", zadnji);
   return;
}

void postavi_pauzu () 
{
   pauza = 1 - pauza;
   return;
}

void prekini () 
{
   printf("zadnji prosti broj: %lu\n", zadnji);
   exit(0);
}

int prost( unsigned long n ) 
{
	unsigned long i, max;
	
	//double temp = n; 
	//ntemp = n;
	if ( ( n%2 ) == 0 ) 
		return 0;

	max = sqrt ( n );
	//max = unsigned long(temp);
	for ( i = 3; i <= max; i += 2 )
		if ( ( n % i ) == 0 )
			return 0;

	return 1; 
}


int main(void)
{
	
  struct itimerval t;

	sigset (SIGALRM, periodicki_ispis);
  signal(SIGTERM, prekini);
	sigset(SIGINT, postavi_pauzu);
  
  /* prvi puta nakon: */
	t.it_value.tv_sec = 0;
	t.it_value.tv_usec = 500000;

	/* nakon prvog puta, periodicki sa periodom: */
	t.it_interval.tv_sec = 0;
	t.it_interval.tv_usec = 500000;

	setitimer ( ITIMER_REAL, &t, NULL );
  
  
	while(1)
	{
		if(prost(broj))
			zadnji=broj;
		broj++;
		while(pauza==1)  pause();
	}
	
	
	return 0;
}
