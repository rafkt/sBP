library(dplyr)
#setwd("Desktop")

csvFile = read.csv("multiplePredictionPerSequence_multiple_Predictors_CACHE.csv")

datasets <- select(filter(filter(filter(csvFile, History.Size=="3"), Cache.Size=="3"), Predictor=="BWT"), Dataset)

counter = 1
for (i in unlist(datasets)){
	for(j in seq(from=3, to=7, by=2)){
		for(k in seq(from=3, to=9, by=3)){
			#on last repetition, j should be 10.
			if (k == 9) k = k + 1
			specificDataset <- filter(csvFile, Dataset == i)
			specificDatasetIK <- filter (specificDataset, History.Size == j)
			specificDatasetIK <- filter (specificDatasetIK, Cache.Size == k)
			
			
			
			jpeg(filename = paste(counter, ".jpeg", sep = ""), width=1200, height=800)

			cost <- specificDatasetIK$Prediction.with.Cache.Cost

			benefit <- specificDatasetIK$Prediction.with.Cache.Benefit

			par(oma = c(4, 1, 1, 1))

			plot(cost, benefit, main = paste(i, " History ", j, " Cache ", k), col=c("red", "yellow", "green", "violet", "orange", "blue", "pink", "cyan")[specificDatasetIK$Predictor])

			par(fig = c(0, 1, 0, 1), oma = c(0, 1, 0, 0), mar = c(0, 0, 0, 0), new = TRUE)

			plot(0, 0, type = "n", bty = "n", xaxt = "n", yaxt = "n")

			legend("bottom", col=c("red", "yellow", "green", "violet", "orange", "blue", "pink", "cyan"), legend = levels(specificDatasetIK$Predictor), bg="white", pch=1, xpd = TRUE, inset = c(0,0), bty = "n", ncol =4, , cex=0.8)

			dev.off()
			
			counter = counter + 1
		}
	}	
}