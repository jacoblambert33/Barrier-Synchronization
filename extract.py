#!/usr/bin/python

from sys import argv
data={}
with open(argv[1],'r') as fi:
	for line in fi:
		t=line.split('=')
		algo=t[0].split('--')[1].split()[0]
		timeElapsed=float(t[-1].split()[0])
		threadNumber=int(t[1].split(',')[0].strip())
		barrierNumber=int(t[2].split()[0][:-1])
		
		if algo not in data:
			data[algo]=[{threadNumber:timeElapsed},{barrierNumber:timeElapsed}]
		else:
			data[algo][0][threadNumber]=data[algo][0].get(threadNumber,0)+timeElapsed
			data[algo][1][barrierNumber]=data[algo][1].get(barrierNumber,0)+timeElapsed
	
with open("fin.csv",'w') as fo:
	for i in data:
		for j in sorted(data[i][0]):
			fo.write(i+","+str(j)+","+str(data[i][0][j]/10.0)+"\n")
		fo.write("\n")
		for k in sorted(data[i][1]):
			fo.write(i+","+str(k)+","+str(data[i][1][k]/10.0)+"\n")
		fo.write("\n")
