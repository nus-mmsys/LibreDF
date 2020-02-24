#!/usr/bin/env python

import sys

if len(sys.argv) < 3 :
    print("usage: "+sys.argv[0]+" <log file> <start time>")
    exit(0)

f = open(sys.argv[1])
resname = "../../plots/rdf/"+sys.argv[1].split(".")[0]+".csv"
res = open(resname, "w")
starttime = int(sys.argv[2])

csv = {}

for line in f:
    linelist = line.split()
    actor = linelist[0]
    it = int(linelist[3])
    start = int(linelist[4])
    exect = int(linelist[7])
    if it in csv :
        csv[it].append([start-starttime, actor, exect])
    else:
        csv[it] = [[start-starttime, actor, exect]]

res.write("iteration,start,actor,exectime\n")
for it in dict(sorted(csv.items())):
    for elem in csv[it]:
        res.write(str(it)+","+str(elem[0])+","
                +elem[1]+","+str(elem[2])+"\n")

f.close()
print(resname+" is created.")
