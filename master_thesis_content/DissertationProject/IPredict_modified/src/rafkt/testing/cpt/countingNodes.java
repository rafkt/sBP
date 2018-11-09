package rafkt.testing.cpt;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import ca.ipredict.database.Item;
import ca.ipredict.database.Sequence;
import ca.ipredict.database.SequenceDatabase;
import ca.ipredict.predictor.CPT.CPTPredictor;
import ca.ipredict.predictor.CPT.PredictionTree;

public class countingNodes {

	public static void main(String[] args) {
				
		Map<Integer, Integer> rangeMod = new HashMap<Integer, Integer>();
		List<Integer> countNodes = new ArrayList<Integer>();
		Set<PredictionTree> countedNodes = new HashSet<PredictionTree>();
		SequenceDatabase sqDB = new SequenceDatabase();
		sqDB.loadFileSPMFFormat(args[0], 0, 0, 0);
		//System.out.println(sqDB.size());
//		for (Sequence i : sqDB.getSequences()){
//			System.out.println(i.toString());
//		}
		CPTPredictor cpt = new CPTPredictor();
		cpt.setTrainingSequences(sqDB.getSequences());
		cpt.Preload();
		//System.out.println(cpt.size());
		int items = 0;
		Set<Item> dItems = new HashSet<Item>();
		for (Sequence s : sqDB.getSequences()){
			items += s.size();
			dItems.addAll(s.getItems());
		}
		
		for(PredictionTree i : cpt.LT.values()){
			for(PredictionTree pt = i; pt != null; pt = pt.Parent){
				//System.out.print(pt.Item.toString() + "-> ");
				if(!countedNodes.contains(pt)) {
					countNodes.add(pt.getChildrenCount());
					//System.out.println(pt.Item.toString() + ": " + pt.getChildrenCount());
					countedNodes.add(pt);
				}
			}
			//System.out.println("\n");
		}
		//System.out.println(countedNodes.size() + " " + countNodes.size());
		for(int i : countNodes){
			//System.out.println(i);
			if (!rangeMod.containsKey(i)) rangeMod.put(i, 1);
			else rangeMod.put(i, rangeMod.get(i) + 1);
		}
		int max = Collections.max(rangeMod.values());
		int commonChNum = 0;
		for (int i : rangeMod.keySet()){
			if (rangeMod.get(i) == max) {
				//System.out.println("Most common children number per node: " + i);
				commonChNum = i;
			}
		}
		//System.out.println("Max children in " + rangeMod.get(Collections.max(countNodes)) + " nodes: " + Collections.max(countNodes));
		
		//For overall children number we could either count the number of parents or like I did count the children right away.
		
		int overallChlidren = 0;
		for (int i : countNodes) overallChlidren += i;
		
		//writing results to csv file
		BufferedWriter out = null;
		try{
			File f = new File("results.csv");
			boolean exists = f.isFile();
		    FileWriter fstream = new FileWriter("results.csv", true);
		    out = new BufferedWriter(fstream);
		    if (!exists) out.write("File Name,Nodes Number,Overall Children Number,Number of Sequences,Number of Items,Distinct Items,Most common children number per node,Max children per node,Number of nodes with max children\n");
		    out.write(args[0] + "," + cpt.size() + "," + overallChlidren + "," + sqDB.size() + "," + items + "," + dItems.size() + "," + commonChNum + "," + Collections.max(countNodes) + "," + rangeMod.get(Collections.max(countNodes)) + "\n");
		    
		}
		catch (IOException e){
		    System.err.println("Error: " + e.getMessage());
		}
		finally{
		    if(out != null) {
		        try {
					out.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
		    }
		}
		System.out.println("Done");
	}
}
