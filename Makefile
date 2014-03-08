all: defaultBarrierOpenMP disseminationBarrier senseReverseBarrier

defaultBarrierOpenMP: defaultBarrierOpenMP.c
	gcc -o defaultBarrierOpenMP defaultBarrierOpenMP.c -fopenmp

disseminationBarrier: dissemination.c
	gcc -o disseminationBarrier dissemination.c -fopenmp -lm

senseReverseBarrier: senseReverseBarrier.c
	gcc -o senseReverseBarrier senseReverseBarrier.c -fopenmp
	
clean:
	rm -f *.o defaultBarrierOpenMP disseminationBarrier senseReverseBarrier

