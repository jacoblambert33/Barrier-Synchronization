#include <omp.h>
#include <stdio.h>

int MAXTHREADCOUNT=0,MAXBARRIERCOUNT=0,globalSense=1,count;

int main(int argc, char** argv)
{
	if (argc==3)
	{
		count=MAXTHREADCOUNT=atoi(*(argv+1)); /*Store a local copy (in MAXTHREADCOUNT) for resetting thread count for future use*/
		MAXBARRIERCOUNT=atoi(*(argv+2));
		printf("In the serial section of code.\nNet threads requested = %d\tNet barriers requested= %d.\n\n",MAXTHREADCOUNT,MAXBARRIERCOUNT);

		printf ("Entering ||\n");

		double startTime[MAXBARRIERCOUNT][MAXTHREADCOUNT];
		double endTime[MAXBARRIERCOUNT][MAXTHREADCOUNT];

		#pragma omp parallel num_threads(MAXTHREADCOUNT) shared(globalSense,count)
		{
			int localSense=1;
			int barrierCounter=MAXBARRIERCOUNT-1;
			while(barrierCounter>=0)
			{
				printf("Thread #%d entering critical section (at Barrier# %d)\n",omp_get_thread_num()+1,barrierCounter+1);
				startTime[barrierCounter][omp_get_thread_num()]=omp_get_wtime();
				#pragma omp barrier
				endTime[barrierCounter][omp_get_thread_num()]=omp_get_wtime();
				printf("Thread #%d exiting critical section (at Barrier# %d)\n",omp_get_thread_num()+1,barrierCounter+1);
				barrierCounter--;
			}
		}

		printf ("\nSafely back in the serial section!\n\n");

		#if 0
		int i=0,j;
		while(i<MAXBARRIERCOUNT)
		{ j=0;
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
		printf("METHOD -- DEFAULT (Thread# = %d, Barrier# = %d) -- Average time taken by threads to complete = %f s\n\n",MAXTHREADCOUNT,MAXBARRIERCOUNT,netVal/MAXTHREADCOUNT);

		return 0;
	}
	return 1;
}