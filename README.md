#Barrier Synchronization using OpenMP

Implementation of two barrier synchronization algorithms using [OpenMP](http://www.openmp.org/) library. These algorithms were then benchmarked against the default barrier provided by OpenMP library.

###Following algorithms were implemented:
####1. Sense reversing centralized barrier:
In a centralized barrier, threads wait (spin) on shared data (variable) while waiting for other threads to complete. Once all threads complete their assigned task(s), they are allowed to move forward to the next portion of execution. In these barriers, it becomes important to note that involved threads constantly 'spin' on a variable - which is shared among all threads (hence can be thought as a hot-spot for contention). In order to ensure that this shared variable (and our execution) is not a victim of race condition, it becomes important for that variable to be guarded - so that only one thread has write access to it at any point in time. In order to ensure this, we use a construct defined in OpenMP - known as semaphore (a.k.a. critical section). The sense-reversing part of the algorithm stems from the fact that value (being 'spun' on) toggles, i.e. every time threads move past barrier. As threads arrive, they wait for the last thread. Once the last thread arrives, it toggles the 'spin' value, releasing the ‘spinning’ threads.

####2. Dissemination barrier
This is another variant of barrier synchronization methodologies - one of the advantages of using it over the one described above, is the decentralized nature of algorithm. In a sense-reversing centralized barrier, it was necessitated by the algorithm to make (participating) threads continually check a shared variable to determine if they are good to move forward - which could end up becoming a source of contention. In disseminating barrier, the communication needed between threads is laid via message passing (conducted per ‘round’), where each round decides which threads communicate and how the execution is furthered - from one threads to another. In each round, thread with id : i communicates with thread(s) with id(s) : (i+2^k)%N (where N= number of participating threads, k=round number). The number of rounds needed for all threads to arrive (at a barrier) is directly proportionate to the logarithm of the number of threads involved (i.e. N).

###Development Environment:
Ubuntu 12.04 LTS (running on 64-bit x86 hardware).

###Experimentation Setup:

1. Used a four-core server from JINX cluster (GaTech). 
2. Default OpenMP library function was used for measuring time elapsed - omp_get_wtime(). 
3. The timings were noted for each thread, during each interaction with barriers. 

These experiments were performed for the following barriers : 

1. Default barrier, 
2. Sense-reversing centralized barrier, and
3. Dissemination barrier. 
 
These values were then aggregated, and a per barrier value was computed - which was then used by us for deriving conclusions.

###Interesting tidbits:

**Experiment 1**

![alt text](https://github.com/rohit-jamuar/Barrier-Synchronization/blob/master/Pngs/1.png "Variation in completion times")

1. I was hoping that the default barrier implementation (i.e. the one provided by the library) would fare much better than the ones implemented. Much to my amazement, the default barrier performed the worst, relatively! Looking at the trends in graph, one could conjecture that the default barrier (being deployed by the library) is something on the lines of sense-reversing centralized barrier.
2. From the graph (depicted above), implementation of dissemination barrier was far more performant than  sense-reversing centralized and default barriers. As far as comparisons between the sense-reversing centralized and dissemination barriers are concerned, one can expect the speedup (in case of dissemination barriers) due to the fact that there are no contentious shared variables to deal with. This idea was further supported by the empirically derived results.
3. Was hoping that as the number of threads increased, the time elapsed (during their execution around a barrier) would grow linearly. To my surprise, I found that there were regions in the graphs, wherein the increase in number of threads yielded plateau like structure rather than a straight 45-degree line. Moreover, from the data yielded by the default barrier, there was a sharp decline of time elapsed with increase in thread count (i.e. when thread count goes from 6 to 8). The results of sense-reversing centralized barrier is coherent with my expectations - as the number of threads grows, so does the contention for the global ‘sense’ variable, leading to increased execution times.

**Experiment 2**

![alt text](https://github.com/rohit-jamuar/Barrier-Synchronization/blob/master/Pngs/2.png "Time Elapsed v/s Barrier Count")

Another follow-up experimentation (with data derived from OpenMP barriers) was to examine the effects of having multiple barriers on execution time. I initially thought that as the number of barriers were increased, the execution time would increase proportionately. Much to surprise, the time elapsed plateau-ed with an increase in barrier count. I attribute caching for this behavior - when the execution started, the working set had to be loaded into caches. Once the working set was loaded, the execution was only dealing with data in main memory (it no longer had to fault, and subsequently fetch anything from disk).

###Compiling and executing code:
1. Run **make**
2. For running binaries individually:

  1. ./senseReverseBarrier *\<number\_of\_threads\>* *\<number\_of\_barriers\>*
  2. ./dissemination *\<number\_of\_threads\>* *\<number\_of\_barriers\>*
  3. ./defaultBarrierOpenMP *\<number\_of\_threads\>* *\<number\_of\_barriers\>*

3. For running binaries via automated script

  1. chmod a+x OpenMPRun.sh
  2. ./OpenMPRun.sh > output.txt
  3. cat output.txt | grep "METHOD --" > filtered.txt
  4. python extract.py filtered.txt

  The results will be stored in a csv named "fin.csv".

