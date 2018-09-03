package rafkt.testing.cpt;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Scanner;

import ca.ipredict.database.Item;
import ca.ipredict.database.Sequence;
import ca.ipredict.database.SequenceDatabase;
import ca.ipredict.predictor.CPT.CPTPredictor;

public class testPrediction {

	public static void main(String[] args) throws FileNotFoundException {
		ArrayList<Sequence> queries = new ArrayList<Sequence>();
		int counter = 0;
		SequenceDatabase sqDB = new SequenceDatabase();
		sqDB.loadFileSPMFFormat(args[0], 0, 0, 0);
		CPTPredictor cpt = new CPTPredictor();
		cpt.setTrainingSequences(sqDB.getSequences());
		cpt.Preload();
		
//		itemList.add(new Item(254));    
//		itemList.add(new Item(253));
//		itemList.add(new Item(153));
//		itemList.add(new Item(193));
//		itemList.add(new Item(151));
//		itemList.add(new Item(225));
		
		Scanner scanner = new Scanner(new BufferedReader(new FileReader(args[1])));
		while (scanner.hasNextLine()) {
			ArrayList<Item> itemList = new ArrayList<Item>();
		    String[] tokens = scanner.nextLine().split(" ");
		    for (String tok : tokens) {
		    	//System.out.println(tok);
		    	itemList.add(new Item(Integer.parseInt(tok)));
		    }
		    queries.add(new Sequence(counter++, itemList));
		    
		}
		scanner.close();
		
		long start = System.nanoTime();
		int iterations = 100;
		for (int i = 0; i < iterations; i++) { //iteration of queries
			for (Sequence q : queries) {
				//System.out.println(cpt.Predict(q));
				cpt.Predict(q);

			}
		}
		double elapsedTimeInMili = (System.nanoTime() - start) * 1.0e-6;
		System.out.println("Overall: " + elapsedTimeInMili + "ms" + " Average: " + elapsedTimeInMili / iterations + "ms " + "Averagely per query: " + elapsedTimeInMili / iterations / queries.size() + "ms");
		
		
		
		
		
		
//		Sequence res = cpt.Predict(query);
//		System.out.println(res);
	}

}
