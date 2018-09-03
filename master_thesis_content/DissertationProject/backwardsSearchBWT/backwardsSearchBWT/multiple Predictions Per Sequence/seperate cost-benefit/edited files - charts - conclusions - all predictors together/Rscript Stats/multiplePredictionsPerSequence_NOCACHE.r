library(dplyr)
#setwd("Desktop")

csvFile = read.csv("multiplePredictionPerSequence_multiple_Predictors_NOCACHE.csv")

datasets <- select(filter(filter(csvFile, History.Size=="3"), Predictor=="BWT"), Dataset)

counter = 1

for (i in unlist(datasets)){
	for(j in seq(from=3, to=7, by=2)){
		specificDataset <- filter(csvFile, Dataset == i)
		specificDatasetI <- filter (specificDataset, History.Size == j)			
			
		jpeg(filename = paste("imgagesStatsNoCache/", counter, ".jpeg", sep = ""), width=1200, height=800)

		barplot(specificDatasetI$Accuracy, ylim = range(0, unlist(select(specificDatasetI, Accuracy)) + 10), main = paste(i, " History ", j, " Accuracy"), ylab = "Percentage", names = specificDatasetI$Predictor, col = c("red", "yellow", "green", "violet", "orange", "blue", "pink", "cyan"))

		dev.off()
		
		counter = counter + 1
	}
}

#example for grouped barplot
#specificDataset2 <- filter(csvFile, Dataset == "BMS")
#specificDatasetI2 <- filter (specificDataset2, History.Size == "3")
#specificDataset <- filter(csvFile, Dataset == "SIGN")
#specificDatasetI <- filter (specificDataset, History.Size == "3")
#specificDatasetI["Accuracy2"] <-specificDatasetI2$Accuracy
#subset <- t(data.frame(specificDatasetI$Accuracy, specificDatasetI$Accuracy2))
#barplot(subset, ylim = range(0, subset +10), main = "Test", ylab = "Percentage", names = specificDatasetI$Predictor, col = c("red", "grey", "yellow", "grey", "green", "grey", "violet","grey", "orange","grey", "blue","grey", "pink","grey", "cyan","grey"), beside=TRUE)
