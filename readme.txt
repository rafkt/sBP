How to run batch experiment with k-folding (exported folds from java IPredict)


on branch of Ipredict go to master_execution_exports_training_folds_queries_eval-suffixes

make clean; make all; make run

on sBP go to subseq_predictor_initial_implementation_validity_test_temporary_change_see_log

parallel ./program /Users/rafaelktistakis/Repositories/Predictors_tests_another_git_clone/Predictors_tests/outputs/{2}.fold.{1}.training.txt /Users/rafaelktistakis/Repositories/Predictors_tests_another_git_clone/Predictors_tests/outputs/{2}.fold.{1}.queries.txt ">>" sbp.{2}.fold.{1}.answers.txt ::: 0 1 2 3 ::: "BMS" "BIBLE_CHAR" "BIBLE_WORD" "SIGN" "MSNBC" "KOSARAK" "FIFA"

back to ipredict, transfer the answers from previous step to outputs/sbp of ipredict and run with

parallel -j 1 java answerValidator {2} {1} ::: 0 1 2 3 ::: "BMS" "BIBLE_CHAR" "BIBLE_WORD" "SIGN" "MSNBC" "KOSARAK" "FIFA"