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

scale = 1000000
latency = data$end[length(data$end)]/scale

plot(data$iteration, (data$end-data$start)/scale, 
     main=name, 
     xlab="iteration", ylab="latency = end(iteration) - start(iteration)",
     type="l", col="blue")

mtext(paste("Total latency = ",toString(latency)," second"))

average = data$iteration[length(data$iteration)]/(data$end[length(data$end)]/scale)

plot(data$iteration, data$iteration/(data$end/scale), 
     main=name, 
     xlab="iteration", ylab="throughput = iteration / end(iteration)",
     type="l", col="blue")

mtext(paste("Average thoughput = ",toString(average)," iteration/second"))
