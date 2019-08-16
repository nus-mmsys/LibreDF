#!/usr/bin/env Rscript

library("ggplot2")

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

ggplot(data, aes(data$iteration, data$exectime/scale, color = data$actor)) +
    geom_point() +
    geom_path(aes(group = data$actor)) +
    labs(title = name, x = "iteration", y = "latency [second]", color = "actor")

#plot(data$iteration, data$exectime/scale, 
#     main=name, 
#     xlab="iteration", ylab="latency [second]",
#     type="l", col="blue")
