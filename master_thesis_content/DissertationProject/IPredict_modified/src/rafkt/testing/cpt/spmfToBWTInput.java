package rafkt.testing.cpt;

import ca.ipredict.database.Sequence;
import ca.ipredict.database.SequenceDatabase;

public class spmfToBWTInput {

	public static void main(String[] args) {
		
		SequenceDatabase sqDB = new SequenceDatabase();
		sqDB.loadFileSPMFFormat(args[0], 0, 0, 0);
		sqDB.writeSequencesToFile(args[0] + "_sequences");

	}

}
