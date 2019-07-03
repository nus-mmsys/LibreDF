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


lastend = head(c(0,data$end),-1)

plot(data$iteration, 1/((data$end-lastend)/scale), 
     main=name, 
     xlab="iteration", ylab="throughput = 1 / (end(iteration) - end(iteration-1))",
     type="l", col="blue")

mtext(paste("Average thoughput = ",toString(average)," iteration/second"))
