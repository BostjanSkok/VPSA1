#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <malloc.h>
#include <windows.h>

struct fromKToN
{
	int k;
	int n;
	int rez; //holds return result of thread

};

int totalPrimes=0;
int totalSum=0;

int IsPrime(int num){

	int lastToCheck = (int) floor(sqrt((double)num));
	int  divisor = 2;
	for ( divisor; divisor <= lastToCheck ;  divisor++)
	{
		if(num%divisor==0)
			return 0 ; 
	}
	return 1;
}
void *HowManyPrimes(void *args)
{
	struct fromKToN* params = (struct fromKToN*) args;
	//printf("%i\n",params->k);
	//printf("%i\n",params->n);


	int i = (params->k);
	for(i; i <= (params->n); i++)
	{
		if(IsPrime(i)==1)
		{
			params->rez++;
		}
	}
	return NULL;

}

void *SumInterval(void *args)
{
	struct fromKToN* params = (struct fromKToN*) args;
	int i = (params->k);
	for(i; i <= (params->n); i++)
	{
		totalSum += i;
	}
	return NULL;
}

void splitIntoThreads(int numOfThreads ,int from , int to )
{
	pthread_t *workers;

	int intervalSize = (int)(to - (from-1))/numOfThreads;
	int i = 0;
	workers = (pthread_t *)malloc(sizeof(pthread_t)*numOfThreads);

	struct fromKToN *intervals = (struct fromKToN *)malloc(numOfThreads*sizeof(struct fromKToN));
	int leftOver = 0;
	for(i; i<numOfThreads;i++)
	{
		intervals[i].k = from + (intervalSize*i);
		intervals[i].k +=leftOver;

		intervals[i].n = (from-1) + (intervalSize * (i+1));
		intervals[i].n += leftOver;
		intervals[i].rez = 0;

		if(i==0)
		{
			leftOver = ((to - (from - 1))%numOfThreads);
			intervals[i].n += leftOver;
		}
		if (pthread_create(&workers[i], NULL, HowManyPrimes, &intervals[i]) != 0){
			perror("t1 create");}

	}

	int k =0;
	for(k; k<numOfThreads;k++)
	{
		if(pthread_join(workers[k],NULL)!=0){
			perror("t1 join");}
		totalPrimes += intervals[k].rez;

	}
	printf("%i", totalPrimes);
}

void NalogaA()
{
	pthread_t t1,t2;

	struct fromKToN s1;
	s1.k=1;
	s1.n=100000;

	if(pthread_create(&t1,NULL,HowManyPrimes,&s1)!=0)
	perror("t1 create");
	if(pthread_create(&t2,NULL,SumInterval,&s1)!=0)
	perror("t2 create");

	if(pthread_join(t1,NULL)!=0)
	perror("t1 join");
	if(pthread_join(t2,NULL)!=0)
	perror("t2 join");
}

void NoThreads()
{
	struct fromKToN s1;
	s1.k=1;
	s1.n=100000;

	HowManyPrimes((void*) &s1);
	SumInterval((void*) &s1);
}
int main() {
	DWORD dwStartTime = GetTickCount();
    DWORD dwElapsed;
	clock_t start = clock();
    splitIntoThreads(4,1,100);
    NalogaA();
	NoThreads();


	
	clock_t end = clock();
	 dwElapsed = GetTickCount() - dwStartTime;
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
	//printf("%.10f",seconds);

	

    printf("It took %d.%3d seconds to complete\n", dwElapsed/1000, dwElapsed - dwElapsed/1000);
	getchar();

}





