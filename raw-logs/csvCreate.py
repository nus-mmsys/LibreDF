#!/usr/bin/env python

import sys

if len(sys.argv) < 2 :
    print("usage: "+sys.argv[0]+" <log file>")
    exit(0)

f = open(sys.argv[1])
resname = sys.argv[1].split(".")[0]+".csv"
res = open(resname, "w")

csv = {}

for line in f:
    linelist = line.split()
    it = int(linelist[2])
    start = int(linelist[4])
    end = int(linelist[6])
    if it in csv :
        if start < csv[it][0] :
            csv[it][0] = start
        if end > csv[it][1] :
            csv[it][1] = end
    else:
        csv[it] = [start, end]

res.write("iteration,start,end\n")

for it in dict(sorted(csv.items())):
    res.write(str(it)+","+str(csv[it][0])+","+str(csv[it][1])+"\n")

f.close()
print(resname+" is created.")
