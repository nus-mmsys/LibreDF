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

ggtheme = theme(plot.title = element_text(size=15),
	axis.title.x = element_text(size=14),
	axis.text.x = element_text(size=12),
	axis.title.y = element_text(size=14),
	axis.text.y = element_text(size=12),
	legend.title = element_text(size=14),
	legend.text = element_text(size=12))

scale = 1000000

ggplot(data, aes(data$iteration, data$exectime/scale, color = data$actor)) +
    geom_path(aes(group = data$actor)) +
    labs(x = "iteration", y = "latency [second]", color = "actor") +
    ggtheme

#plot(data$iteration, data$exectime/scale, 
#     main=name, 
#     xlab="iteration", ylab="latency [second]",
#     type="l", col="blue")
