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

plot(data$iteration, data$exectime/scale, 
     main=name, 
     xlab="iteration", ylab="latency [second]",
     type="l", col="blue")
