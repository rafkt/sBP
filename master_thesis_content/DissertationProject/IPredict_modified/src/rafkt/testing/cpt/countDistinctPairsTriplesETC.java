package rafkt.testing.cpt;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;

import ca.ipredict.database.Sequence;
import ca.ipredict.database.SequenceDatabase;

public class countDistinctPairsTriplesETC {

	public static void main(String[] args) {
		
		SequenceDatabase sqDB = new SequenceDatabase();
		sqDB.loadFileSPMFFormat(args[0], 0, 0, 0);
		
		ArrayList<ArrayList<Sequence>> pairsTriplesETC = new ArrayList<ArrayList<Sequence>>();
		int[] overall_tuple_counter = new int[8];
		
		for (int i = 2; i < 10; i++){
			for (Sequence s : sqDB.getSequences()){
				if (s.size() >= i) {
					Sequence tuple = new Sequence(i);
					for (int j = 0; j < i; j++) tuple.addItem(s.get(j));
					try {
						if (!pairsTriplesETC.get(i - 2).contains(tuple)){
							pairsTriplesETC.get(i - 2).add(tuple);
							//System.out.println(tuple);
						}		
					} catch (IndexOutOfBoundsException e) {
						ArrayList<Sequence> list = new ArrayList<Sequence>();
						pairsTriplesETC.add(list);
						pairsTriplesETC.get(i - 2).add(tuple);
						//System.out.println(tuple);
					}
					overall_tuple_counter[i - 2]++;
				}
			}
		}
		
		
		BufferedWriter out = null;
		try{
			File f = new File("Tuple_results.txt");
			boolean exists = f.isFile();
		    FileWriter fstream = new FileWriter("Tuple_results.txt", true);
		    out = new BufferedWriter(fstream);
		    out.write("Filename: " + args[0] + "\n");
		    for (ArrayList<Sequence> s : pairsTriplesETC) out.write(s.get(0).size() + " Tuple: " + s.size() + " distinct, " + overall_tuple_counter[s.get(0).size() - 2] + " overall" + "\n");
		    
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
