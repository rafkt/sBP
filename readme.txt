The base implementation of the subseq predictor is in folder sBP/simple_bs_predictor under branch subseq_predictor_initial_implementation_validity_test.

Any changes for my current workflow  will be posted here.

Core source files:
backwardSearchBWT.cpp is all about offering the main BWT functionalities. Perhaps this will be the file with most optimisations (and/or more crucial bugs). 
subseqPredictor.cpp implements the predictor by generating all the sub-queries and calling the different BWT functions to search for patterns and obtain consequents.
suffixArray.cpp implements a suffix array and generated the BWT which is put in an sdsl WT at the end. This file has remained the same since my master Thesis

Use make all command to compile from source code, using the provided Makefile (it is required to have pre-installed sdsl-lite).

For up to date execution see end of this readme file.

Run with: ./program ../validity_datasets/FIFA.training.txt ../validity_datasets/queries.Fifa.txt in order to train on FIFA.training.txt and use queries from quries.Fifa.txt
The execution will report memory usage and summary of the test time for the entire set of queries.
Under ./validity_datasets you will find a couple more datasets to try; But for now, the queries sets are long and it takes a few hours to fully run. (should shrink the queries sets until we find the bug/optimise implementation)


Current Work:

Accuracies have been validated according to the results I had obtained in the Java (prototyped) sBP predictor. I can reproduce the accuracies that I have reported Succinct Burrows-Wheeler Transform Predictor (sBP) report (was written back in August 2018).

Now I am running peformance experiments to compare, memmory and speed to CPT/+ and other predictors.

After I finish with my performance experiments I will try to add SPice baseline (spectral learning) into the comparisons of accuracy and performance. Thus we can obtain a first understanding on how our predictor (CPT/+) behave with neural networks, and if it behaves worse then I have to investigate it further.



Past Work:

I am currently working on a new branch (since branch subseq_predictor_initial_implementation_validity_test has a stable predictor).
I am trying to implement the predictor in a way that for sub-queries of <??abc> it only does <abc>. Current tests showed massive speed improvement. However, I have a bug which results to a 5% drops of accuracy that at the moment I am investigating.
See branch subseq_predictor_initial_implementation_validity_test_temporary_change_see_log


DONE -> Working on scan function under backwardSearchBWT.cpp; Current implementation depends on alphabet size
DONE -> Working on backwardTraversal function; It uses linear search. I will convert the search to a binary search
DONE -> predict method under subseqPredictor.cpp has some un-needed getRange calls; I will get rid of them
DONE -> Under subseqPredictor.cpp I will put a couple of counters that will report (average per query) how many sub-queries searches have been done, how many ranges obtained and how many consequents eventually returned
DONE -> Will repeat a run for Fifa or Kosarak
DONE -> Will quickly check if prediction results remain valid and no new bugs introduced


=====================================================================================================================================================================
How to run batch experiment with k-folding (exported folds from java IPredict)


on branch of Ipredict go to master_execution_exports_training_folds_queries_eval-suffixes

make clean; make all; make run

on sBP go to subseq_predictor_initial_implementation_validity_test_temporary_change_see_log

parallel ./program /Users/rafaelktistakis/Repositories/Predictors_tests_another_git_clone/Predictors_tests/outputs/{2}.fold.{1}.training.txt /Users/rafaelktistakis/Repositories/Predictors_tests_another_git_clone/Predictors_tests/outputs/{2}.fold.{1}.queries.txt ">>" sbp.{2}.fold.{1}.answers.txt ::: 0 1 2 3 ::: "BMS" "BIBLE_CHAR" "BIBLE_WORD" "SIGN" "MSNBC" "KOSARAK" "FIFA"

back to ipredict, transfer the answers from previous step to outputs/sbp of ipredict and run with

parallel -j 1 java answerValidator {2} {1} ::: 0 1 2 3 ::: "BMS" "BIBLE_CHAR" "BIBLE_WORD" "SIGN" "MSNBC" "KOSARAK" "FIFA"
=====================================================================================================================================================================
