#!/usr/bin/env python

import sys

if len(sys.argv) < 2 :
    print("usage: "+sys.argv[0]+" <log file>")
    exit(0)

f = open(sys.argv[1])
resname = "../../plots/rdf/"+sys.argv[1].split(".")[0]+".csv"
res = open(resname, "w")

csv = {}

for line in f:
    linelist = line.split()
    step = int(linelist[2])
    #it = int(linelist[3])
    start = int(linelist[4])
    end = int(linelist[5])
    execution = int(linelist[6])
    if step in csv :
        if start < csv[step][0] :
            csv[step][0] = start
        if end > csv[step][1] :
            csv[step][1] = end
        csv[step][2] += execution
    else:
        csv[step] = [start, end, execution]

res.write("step,start,end,latency,execution\n")
starttime = csv[1][0]
for step in dict(sorted(csv.items())):
    res.write(str(step)+","
              +str(csv[step][0]-starttime)+","
              +str(csv[step][1]-starttime)+","
              +str(csv[step][1]-csv[step][0])+","
              +str(csv[step][2])+"\n")

f.close()
print(resname+" is created.")
