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

plot(data$iteration, data$end-data$start, 
     main=name, 
     xlab="iteration", ylab="latency(iteration)",
     type="l", col="blue")

