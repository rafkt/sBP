library(dplyr)
#setwd("Desktop")

csvFile = read.csv("5%SUPP_100UNITED_CACHE_multiplePredictionPerSequence_multiple_Predictors_CACHE.csv")

datasets <- select(filter(filter(csvFile, History.Size=="3"), Predictor=="BWT"), Dataset)

counter = 1
for (i in unlist(datasets)){
	for(j in seq(from=3, to=7, by=2)){
		specificDataset <- filter(csvFile, Dataset == i)
		specificDatasetIK <- filter (specificDataset, History.Size == j)
		specificPredictorIKP <- filter (specificDatasetIK, Predictor == "AKOM")
			
			
		jpeg(filename = paste(counter, ".jpeg", sep = ""), width=1200, height=800)

		cost <- specificDatasetIK$Prediction.with.Cache.Cost

		benefit <- specificDatasetIK$Prediction.with.Cache.Benefit
			
		cost <- c(cost, specificPredictorIKP$Simple.LRU.Cache.Cost)
			
		benefit <- c(benefit, specificPredictorIKP$Simple.LRU.Cache.Benefit)
			

		par(oma = c(4, 1, 1, 1))
			
		predictors <- specificDatasetIK$Predictor
		predictors <- factor(append(as.character(predictors),"No Predictor"))
		predictors <- factor(predictors,levels(predictors)[c(1:7, 9, 8)])

		plot(cost, benefit, main = paste(i, " History ", j), col=c("red", "yellow", "green", "violet", "orange", "blue", "pink", "cyan", "purple")[predictors])

		par(fig = c(0, 1, 0, 1), oma = c(0, 1, 0, 0), mar = c(0, 0, 0, 0), new = TRUE)

		plot(0, 0, type = "n", bty = "n", xaxt = "n", yaxt = "n")

		legend("bottom", col=c("red", "yellow", "green", "violet", "orange", "blue", "pink", "cyan", "purple"), legend = levels(predictors), bg="white", pch=1, xpd = TRUE, inset = c(0,0), bty = "n", ncol =4, , cex=0.8)

		dev.off()
			
		counter = counter + 1
	}	
}