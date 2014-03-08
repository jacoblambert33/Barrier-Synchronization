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
Used a four-core server from JINX cluster (GaTech). After deploying code to cluster, experimentation centered around capturing variations in execution times with thread count and with number of barriers. Varying number of barriers from 1 to 900 -  with increments of 100, and number of threads from 2 to 8 - as it wouldn't make sense to go beyond 8, considering the fact that we were experimenting on a single four-core server (which guaranteed two hardware threads / core). Default OpenMP library function was used for measuring time elapsed - omp_get_wtime(). The timings were noted for each thread, during each interaction with barriers. These experiments were performed for the following barriers :

1. default barrier, 
2. sense-reversing centralized barrier, and
3. dissemination barrier. 

These values were then aggregated, and a per barrier value was computed - which was then used by us for deriving conclusions.



