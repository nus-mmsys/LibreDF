#!/usr/bin/env Rscript

library("ggplot2")
#library("gganimate")

args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
	print("usage: ./plot <csv file>")
	quit()
}

data <- read.csv(args[1])

name <- unlist(strsplit(args[1],"[.]"))[1]

#print(data)


scale = 1000000
latency = data$end[length(data$end)]/scale

ggtheme = theme(plot.title = element_text(size=15),
	axis.title.x = element_text(size=14),
	axis.text.x = element_text(size=12),
	axis.title.y = element_text(size=14),
	axis.text.y = element_text(size=12))

#plot(data$it, (data$end-data$start)/scale, 
#     main=name, 
#    xlab="iteration", ylab="latency = end(iteration) - start(iteration)",
#     xlab="token", ylab="latency [second]",
#     type="l", col="blue")

pdf(paste(name,"_lat.pdf", sep=""))

ggplot(data, aes(data$start/scale, (data$latency)/scale)) +
     geom_path(colour = "blue") +
     labs(x="time [second]", y="latency [second]") +
     ggtheme

#print(paste("Total latency = ",toString(latency)," second"))

#average = data$it[length(data$it)]/(data$end[length(data$end)]/scale)

#plot(data$it, data$sol/((data$end-lastend)/scale), 
#     main=name, 
#    xlab="iteration", ylab="throughput = 1 / (end(iteration) - end(iteration-1))",
#     xlab="token", ylab="throughput [token/second]",
#     type="l", col="blue")

anim <- FALSE
num <- 500

if (!anim) {

  data <- read.csv(args[1])
  #lastend = head(c(0,data$end),-1)
  
  p <- ggplot(data, aes(data$start/scale,  data$sol/((data$period)/scale))) +
     geom_path(colour = "blue") +
     labs(x="time [second]", y="throughput [token/second]") +
     ggtheme

  ggsave(paste(name,"_thr.pdf", sep=""))

} else {

  for (i in 2:num) {
  
    data <- read.csv(args[1], nrows=i)
    #lastend = head(c(0,data$end),-1)
  
    p <- ggplot(data, aes(data$start/scale, data$sol/((data$period)/scale))) +
       geom_path(colour = "blue") +
       labs(x="iteration", y="throughput [token/second]") +
       ggtheme

    oname = paste(sprintf("%04d",i), ".png", sep="")
    ggsave(oname, width=6, height=6)
  
    #a <- animate(p, renderer = ffmpeg_renderer())
    #anim_save("animation.mp4", a)
  }
}

#print(paste("Average thoughput = ",toString(average)," iteration/second"))
#print(paste("Average thoughput = ",toString(average)," token/second"))
