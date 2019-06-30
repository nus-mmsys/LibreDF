#!/usr/bin/env Rscript

args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
	print("usage: ./plot <csv file>")
	quit()
}

data <- read.csv(args[1])

name <- unlist(strsplit(args[1],"[.]"))[1]

#print(data)

pdf(paste(name,".pdf", sep=""))

latency = data$end[length(data$end)]

plot(data$iteration, data$end-data$start, 
     main=name, 
     xlab="iteration", ylab="latency(iteration)",
     type="l", col="blue")

mtext(paste("Total latency = ",toString(latency)))

average = data$end[length(data$end)]/data$iteration[length(data$iteration)]

plot(data$iteration, (data$end-average)/data$iteration, 
     main=name, 
     xlab="iteration", ylab="throughput(0,iteration)",
     type="l", col="blue")

mtext(paste("Average thoughput = ",toString(average)))
