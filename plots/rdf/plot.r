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
latency = data$end[length(data$end)]/scale

#plot(data$step, (data$end-data$start)/scale, 
#     main=name, 
#    xlab="iteration", ylab="latency = end(iteration) - start(iteration)",
#     xlab="token", ylab="latency [second]",
#     type="l", col="blue")

ggplot(data, aes(data$step, (data$end-data$start)/scale)) +
     geom_path(colour = "blue") +
     labs(title = name, x="token", y="latency [second]", color="blue")

#mtext(paste("Total latency = ",toString(latency)," second"))

average = data$step[length(data$step)]/(data$end[length(data$end)]/scale)

lastend = head(c(0,data$end),-1)

#plot(data$step, 1/((data$end-lastend)/scale), 
#     main=name, 
#    xlab="iteration", ylab="throughput = 1 / (end(iteration) - end(iteration-1))",
#     xlab="token", ylab="throughput [token/second]",
#     type="l", col="blue")

ggplot(data, aes(data$step,  1/((data$end-lastend)/scale))) +
     geom_path(colour = "blue") +
     labs(title = name, x="token", y="throughput [second]")


#mtext(paste("Average thoughput = ",toString(average)," iteration/second"))
#mtext(paste("Average thoughput = ",toString(average)," token/second"))
