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
    actor = linelist[0]
    it = int(linelist[3])
    exect = int(linelist[7])
    if it in csv :
        csv[it].append([actor, exect])
    else:
        csv[it] = [[actor, exect]]

res.write("iteration,actor,exectime\n")
for it in dict(sorted(csv.items())):
    for elem in csv[it]:
        res.write(str(it)+","+elem[0]+","+str(elem[1])+"\n")

f.close()
print(resname+" is created.")
