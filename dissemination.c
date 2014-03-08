#include <stdio.h>
#include <omp.h>
#include <math.h>
#define THREAD_COUNT_UPPER_LIMIT 10000
#define LOG(a) (log((double)(a))/log(2.0))


int MAXTHREADCOUNT, MAXBARRIERCOUNT,roundCount;

typedef struct flags
{
char myflags[2][THREAD_COUNT_UPPER_LIMIT];
char *partnerflags[2][THREAD_COUNT_UPPER_LIMIT];
}flags;


void dissemination_barrier(flags *localflags, int *sense, int *parity)
{
	int i=0;

	while(i<roundCount)
	{
		#pragma omp critical
		{
			*localflags->partnerflags[*parity][i]=*sense;	
		}
		while(localflags->myflags[*parity][i]!=*sense);
		i++;
	}

	if(*parity) *sense=~*sense;
	*parity=1-*parity;
}

void initializeBarrier(flags* allnodes)
{
	int x=0;
	while(x<MAXTHREADCOUNT)
	{
		int k=0;
		while (k<roundCount)
		{
			allnodes[x].myflags[1][k]=allnodes[x].myflags[0][k]=0; /*Initialization*/
			k++;
		}
		x++;
	}
}

int main(int argc, char **argv)
{

	if(argc==3)
	{
		MAXTHREADCOUNT = atoi(argv[1]);
		MAXBARRIERCOUNT = atoi(argv[2]);

		if (MAXTHREADCOUNT>THREAD_COUNT_UPPER_LIMIT) MAXTHREADCOUNT=THREAD_COUNT_UPPER_LIMIT;

		flags allnodes[MAXTHREADCOUNT];
		roundCount=ceil(LOG(MAXTHREADCOUNT));

		double startTime[MAXBARRIERCOUNT][MAXTHREADCOUNT];
		double endTime[MAXBARRIERCOUNT][MAXTHREADCOUNT];

		printf("\nEntering ||\n\n");

		#pragma omp parallel num_threads(MAXTHREADCOUNT) shared(allnodes, roundCount)
		{
			int parity=0; 
			int sense=1; 
			int j,i=0;
			int curTID=omp_get_thread_num();
			
			flags *localflags=allnodes+curTID;

			#pragma omp single nowait
			{
				initializeBarrier(allnodes);
			}

			while (i<MAXBARRIERCOUNT)
			{
				#pragma omp critical
				{
					j=0;
					while(j<MAXTHREADCOUNT)
					{
						int k=0;
						while(k<roundCount)
						{
							if(j==(curTID+(int)ceil(pow(2,k)))%MAXTHREADCOUNT)
							{
								allnodes[curTID].partnerflags[0][k] =  &allnodes[j].myflags[0][k];
								allnodes[curTID].partnerflags[1][k] =  &allnodes[j].myflags[1][k];
							}
							k++;	
						}
						j++;
					}
				}

				printf("Thread #%d entering critical section (using Barrier# %d)\n",curTID+1,i+1);
				startTime[i][curTID]=omp_get_wtime();
				dissemination_barrier(localflags, &sense, &parity);
				endTime[i][curTID]=omp_get_wtime();
				printf("Thread #%d exiting critical section (using Barrier# %d)\n",omp_get_thread_num()+1,i+1);
				++i;
			}
		}
		
		printf ("\nSafely back in the serial section!\n\n");

		#if 0
		int i=0,j;
		while(i<MAXBARRIERCOUNT)
		{ 
			j=0;
			printf("At Barrier #%d:\n",i+1);
			while(j<MAXTHREADCOUNT)
			{
				printf("Time taken by Thread #%d to complete = %f\n",j+1,endTime[i][j]-startTime[i][j]);
				j++;
			}
			i++;
		}

		printf("\n\n");

		i=0;
		while(i<MAXTHREADCOUNT)
		{
			j=0;
			float sum=0.0;
			while(j<MAXBARRIERCOUNT)
			{
				sum+=endTime[j][i]-startTime[j][i];
				j++;
			}
			printf("Average time taken by Thread #%d to complete = %f ms\n",i+1,sum/(float)MAXBARRIERCOUNT);
			i++;
		}
		#endif 

		int i=0,j;
		float netVal=0.0,sum=0.0;
		while(i<MAXTHREADCOUNT)
		{
			j=0,sum=0.0;
			while(j<MAXBARRIERCOUNT)
			{
				sum+=endTime[j][i]-startTime[j][i];
				j++;
			}
			netVal+=sum/(float)MAXBARRIERCOUNT;
			i++;
		}
		printf("METHOD -- DISSEMINATION (Thread# = %d, Barrier# = %d) -- Average time taken by threads to complete = %f s\n\n",MAXTHREADCOUNT,MAXBARRIERCOUNT,netVal/MAXTHREADCOUNT);

		return 0;
	}
	return 1;
}