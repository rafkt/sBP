package rafkt.testing;

import java.util.ArrayList;

import ca.ipredict.database.Item;
import ca.ipredict.database.Sequence;
import ca.ipredict.database.SequenceDatabase;
import ca.ipredict.predictor.CPT.CPTPredictor;

public class testPrediction {

	public static void main(String[] args) {
ArrayList<Item> itemList = new ArrayList<Item>();
		
		itemList.add(new Item(602));
		
		
		
		SequenceDatabase sqDB = new SequenceDatabase();
		sqDB.loadFileSPMFFormat(args[0], 20450, 0, 100);
		CPTPredictor cpt = new CPTPredictor();
		cpt.setTrainingSequences(sqDB.getSequences());
		cpt.Preload();
		Sequence query = new Sequence(1, itemList);
		Sequence res = cpt.Predict(query);
		System.out.println(res);

	}

}
