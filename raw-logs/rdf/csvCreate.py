#!/usr/bin/env python

import sys

if len(sys.argv) < 2 :
    print("usage: "+sys.argv[0]+" <log file>")
    exit(0)

f = open(sys.argv[1])
resname = "../../plots/rdf/"+sys.argv[1].split(".")[0]+".csv"
res = open(resname, "w")

csv = {}
solutions = {}

for line in f:
    linelist = line.split()
    actname = linelist[0]
    #step = int(linelist[2])
    it = int(linelist[3])
    start = int(linelist[4])
    end = int(linelist[5])
    #execution = int(linelist[6])
    #if step in csv :
    #    if start < csv[step][0] :
    #        csv[step][0] = start
    #    if end > csv[step][1] :
    #        csv[step][1] = end
    #    csv[step][2] += execution
    #else:
    #    csv[step] = [start, end, execution]

    if it in csv :

        if actname in solutions[it] :
            solutions[it][actname] += 1
        else :
            solutions[it] = {actname : 1}

        if start < csv[it][0] :
            csv[it][0] = start
        if end > csv[it][1] :
            csv[it][1] = end
            csv[it][2] = solutions[it][actname]
        #csv[it][2] += execution
    else:
        csv[it] = [start, end, 1]
        solutions[it] = {actname : 1}

#res.write("step,start,end,latency,execution\n")
#starttime = csv[1][0]
#for step in dict(sorted(csv.items())):
#    res.write(str(step)+","
#              +str(csv[step][0]-starttime)+","
#              +str(csv[step][1]-starttime)+","
#              +str(csv[step][1]-csv[step][0])+","
#              +str(csv[step][2])+"\n")

res.write("it,start,end,latency,sol,period\n")
starttime = csv[1][0]
period = csv[2][1]-csv[1][1]
lastsol = 0
for it in dict(sorted(csv.items())):
    if csv[it][2] != lastsol:
	lastsol = csv[it][2]
	period = csv[it+1][1] - csv[it][1]
	continue
    res.write(str(it)+","
              +str(csv[it][0]-starttime)+","
              +str(csv[it][1]-starttime)+","
              +str(csv[it][1]-csv[it][0])+","
              +str(csv[it][2])+","
	      +str(int(period))+"\n")
    period = 0.6*period + 0.4*(csv[it][1]-csv[it-1][1])  

f.close()
print(resname+" is created.")
