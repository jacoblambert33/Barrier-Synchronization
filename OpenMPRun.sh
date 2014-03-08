#PBS -q cs6210
#PBS -l nodes=1:fourcore
#PBS -l walltime=00:20:00
#PBS -N openMPRun@!@!@!

cd $PBS_O_WORKDIR
cat $PBS_NODEFILE

echo "*** STARTED: `date` on `hostname` ***"
echo $PWD
cat $PBS_NODEFILE
echo -e "\n\n"

# Run the program

echo "Default Barrier"
for i in 1 100 200 300 400 500 600 700 800 900
do
	echo ""
	for j in {2..8}
	do
		./defaultBarrierOpenMP $j $i
		echo ""
	done
done

echo "Sense Reversed Barrier"
for i in 1 100 200 300 400 500 600 700 800 900
do
	echo ""
	for j in {2..8}
	do
		./senseReverseBarrier $j $i
		echo ""
	done
done

echo "Dissemination Barrier"
for i in 1 100 200 300 400 500 600 700 800 900
do
	echo ""
	for j in {2..8}
	do
		./disseminationBarrier $j $i
		echo ""
	done
done

echo "*** COMPLETED: `date` on `hostname` ***"

# eof
