#!/bin/bash

clear

echo "Starting BWT CACHE experiments"

./run_accuracy_tests.sh

echo "Finished BWT CACHE experiments"

mv csvProducerResults.csv csvProducerResultsCACHE.csv

#sed 's/CACHE true/CACHE false/g' k-folder_evaluator_run.cpp > tmp.cpp
#mv tmp.cpp k-folder_evaluator_run.cpp

#echo "Tweaked BWT code for NO cache experiments"
#echo "Starting BWT NO cache experiments"

#./run_accuracy_tests.sh

#echo "Finished BWT NO cache experiments"

#mv csvProducerResults.csv csvProducerResultsNOCACHE.csv
