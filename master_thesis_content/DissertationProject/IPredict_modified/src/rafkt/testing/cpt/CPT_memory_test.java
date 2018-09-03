package rafkt.testing.cpt;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.util.Collections;

import ca.ipredict.database.SequenceDatabase;
import ca.ipredict.predictor.CPT.CPTPredictor;

public class CPT_memory_test {

	public static void main(String[] args) {
		
		BufferedWriter out = null;
		try{
		    FileWriter fstream = new FileWriter(args[0] + "_memoryUsageResults.txt", true);
		    out = new BufferedWriter(fstream);
		    
		    out.write("Starting - nothing stored\n");
			printMemoryUsage(out);
			
			callGC(out);
	        
	        printMemoryUsage(out);
	        
	        callGC(out);
	        
	        SequenceDatabase sqDB = new SequenceDatabase();
			sqDB.loadFileSPMFFormat(args[0], 0, 0, 0);
			CPTPredictor cpt = new CPTPredictor();
			cpt.setTrainingSequences(sqDB.getSequences());
			cpt.Preload();
			
			callGC(out);
			out.write("Stored: CPT.II, CPT.LT, CPT.PT, Sequences\n");
			printMemoryUsage(out);
			
			cpt.II = null;
			cpt.LT = null;
			cpt.trainingSequences = null;
			sqDB = null;
			
			callGC(out);
			callGC(out);
			callGC(out);
			out.write("Cleared: CPT.II, CPT.LT, Sequences\n");
			out.write("On Memory: CPT.PT\n");
			out.write("keep in mind that from CPT prediction removed int nodeNumber for more accurate results\n");
	        
	        printMemoryUsage(out);
		    
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

	private static void callGC(BufferedWriter out) throws IOException {
		Object obj = new Object();
        WeakReference ref = new WeakReference<Object>(obj);
        obj = null;
        while(ref.get() != null) {
          System.gc();
        }
        out.write("GC called\n");
	}

	private static int printMemoryUsage(BufferedWriter out) throws IOException {
		int kb = 1024;
        
        //Getting the runtime reference from system
        Runtime runtime = Runtime.getRuntime();
         
        out.write("##### Heap utilisation statistics [KB] #####\n");
         
        //Print used memory
        out.write("Used Memory:"
            + (runtime.totalMemory() - runtime.freeMemory()) / kb + " KBytes\n");
 
        //Print free memory
        out.write("Free Memory:"
            + runtime.freeMemory() / kb + " KBytes\n");
         
        //Print total available memory
        out.write("Total Memory:" + runtime.totalMemory() / kb + " KBytes\n");
 
        //Print Maximum available memory
        out.write("Max Memory:" + runtime.maxMemory() / kb + " KBytes\n");
		return kb;
	}

}
