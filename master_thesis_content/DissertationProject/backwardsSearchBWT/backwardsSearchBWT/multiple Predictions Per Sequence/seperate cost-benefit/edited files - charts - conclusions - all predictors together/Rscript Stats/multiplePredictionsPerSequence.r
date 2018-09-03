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
			
			
			
			jpeg(filename = paste("imgagesStatsCache/", counter, ".jpeg", sep = ""), width=1200, height=800)

			barplot(specificDatasetIK$Cost.Increase.from.NoCace.to.Prediction.Cache, ylim = c(0,100), main = paste(i, " History ", j, " Cache ", k, " Cost Increase from No Cache to Prediction Cache"), ylab = "Percentage increase", names = specificDatasetIK$Predictor, col = c("red", "yellow", "green", "violet", "orange", "blue", "pink", "cyan"))

			dev.off()
			
			counter = counter + 1

			jpeg(filename = paste("imgagesStatsCache/", counter, ".jpeg", sep = ""), width=1200, height=800)

			barplot(specificDatasetIK$Prediction.in.Cache.Benefit.Percentage, ylim = range(0, unlist(select(specificDatasetIK, Prediction.in.Cache.Benefit.Percentage)) + 10), main = paste(i, " History ", j, " Cache ", k, "Prediction in Cache Benefit Percentage"), ylab = "Percentage increase", names = specificDatasetIK$Predictor, col = c("red", "yellow", "green", "violet", "orange", "blue", "pink", "cyan") )

			dev.off()
			
			counter = counter + 1
			
		}
	}
}