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

ggtheme = theme(plot.title = element_text(size=15),
	axis.title.x = element_text(size=14),
	axis.text.x = element_text(size=12),
	axis.title.y = element_text(size=14),
	axis.text.y = element_text(size=12),
	legend.title = element_text(size=14),
	legend.text = element_text(size=12))

scale = 1000000

anim <- FALSE

if (!anim) {

  ggplot(data, aes(data$iteration, data$exectime/scale, color = data$actor)) +
    geom_path(aes(group = data$actor)) +
    labs(x = "iteration", y = "latency [second]", color = "actor") +
    ggtheme
 
  ggsave(paste(name,".pdf", sep=""))

} else {

  for (i in 2:1000) {
  
    data <- read.csv(args[1], nrows=i)
 
    ggplot(data, aes(data$iteration, data$exectime/scale, color = data$actor)) +
      geom_path(aes(group = data$actor)) +
      labs(x = "iteration", y = "latency [second]", color = "actor") +
      ggtheme
 
    oname = paste(sprintf("%04d",i), ".png", sep="")
    ggsave(oname, width=6, height=6)
  
    #a <- animate(p, renderer = ffmpeg_renderer())
    #anim_save("animation.mp4", a)
  }
}

#plot(data$iteration, data$exectime/scale, 
#     main=name, 
#     xlab="iteration", ylab="latency [second]",
#     type="l", col="blue")
