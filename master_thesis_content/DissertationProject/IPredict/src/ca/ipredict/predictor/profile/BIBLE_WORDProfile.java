package ca.ipredict.predictor.profile;

public class BIBLE_WORDProfile extends Profile {

	@Override
	public void Apply() {
		//Global parameters
		//Pre-processing
		sequenceMinSize = 6;
		sequenceMaxSize = 999;
		removeDuplicatesMethod = 1;
		consequentSize = 1; 
		windowSize = 5; 
		
		///////////////
		//CPT parameters
		//Training
		splitMethod = 1; //0 for no split, 1 for basicSplit, 2 for complexSplit
		splitLength = 8; // max tree height
		
		//Prediction
		recursiveDividerMin = 0; //should be >= 0 and < recursiveDividerMax 
		recursiveDividerMax = 5; //should be > recusiveDividerMax and < windowSize
		
		//best prediction from the count table
		firstVote = 1; //1 for confidence, 2 for lift
		secondVote = 0; //0 for none, 1 for support, 2 for lift
		voteTreshold = 0.0; //confidence threshold to validate firstVote, else it uses the secondVote 
		
		//Countable weight system
		countTableWeightMultiplier = 2; // 0 for no weight (1), 1 for 1/targetSize, 2 for level/targetSize
		countTableWeightDivided = 1; // 0 for no divider, 1 for x/(#ofBranches for this sequence)
		
		//Others
		useHashSidVisited = true;
		branchTraversalTopToBottom = false; //used for branches with duplicates, set to true to allow with duplicates
		removeUnknownItemsForPrediction = true; //remove items that were never seen before from the Target sequence before LLCT try to make a prediction
	}

}
