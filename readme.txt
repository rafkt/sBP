The base implementation of the subseq predictor is in folder sBP/simple_bs_predictor under branch subseq_predictor_initial_implementation_validity_test.

Any changes for my current workflow  will be posted here.

Use make all command to compile from source code, using the provided Makefile (it is required to have pre-installed sdsl-lite).
Run with: ./program ../validity_datasets/FIFA.training.txt ../validity_datasets/queries.Fifa.txt in order to train on FIFA.training.txt and use queries from quries.Fifa.txt
The execution will report memory usage and summary of the test time for the entire set of queries.
Under ./validity_datasets you will find a couple more datasets to try; But for now, the queries sets are long and it takes a few hours to fully run. (should shrink the queries sets until we find the bug/optimise implementation)


Current Work:

1. Working on scan function under backwardSearchBWT.cpp; Current implementation depends on alphabet size
2. Working on backwardTraversal function; It uses linear search. I will convert the search to a binary search
3. predict method under subseqPredictor.cpp has some un-needed getRange calls; I will get rid of them
4. Under subseqPredictor.cpp I will put a couple of counters that will report (average per query) how many sub-queries searches have been done, how many ranges obtained and how many consequents eventually returned
5. Will repeat a run for Fifa or Kosarak
