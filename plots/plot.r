#!/usr/bin/env Rscript

args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
	print("usage: ./plot <csv file>")
	quit()
}

data <- read.csv(args[1])

#print(data)

plot(data$iteration, data$end-data$start, 
     main="Iteration latency", ylab="latency(x)",
     type="l", col="blue")

