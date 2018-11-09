#!/bin/bash
rm -rf accuracyTest.csv
make all
echo "Starting BWT experiments"
./k_fold_run ./convertedSequencesForBWT/BIBLE.txt_sequences.txt 5000 0 5
./k_fold_run ./convertedSequencesForBWT/BMS1_spmf.txt_sequences.txt 5000 4 99
echo "20% COMPLETED"
./k_fold_run ./convertedSequencesForBWT/FIFA.txt_sequences.txt 5000 1 99
echo "30% COMPLETED"
./k_fold_run ./convertedSequencesForBWT/Kosarak_converted.txt_sequences.txt 20000 3 99
echo "40% COMPLETED"
./k_fold_run ./convertedSequencesForBWT/LEVIATHAN.txt_sequences.txt 5000 0 5
./k_fold_run ./convertedSequencesForBWT/MSNBC.txt_sequences.txt 5000 1 5
echo "60% COMPLETED"
./k_fold_run ./convertedSequencesForBWT/SIGN.txt_sequences.txt 5000 3 99
./k_fold_run ./convertedSequencesForBWT/BMS2_converted_sequences.txt 5000 4 99
echo "80% COMPLETED"
./k_fold_run ./convertedSequencesForBWT/NASA_access_log_Aug95_converted_sequences.txt 5000 1 99
./k_fold_run ./convertedSequencesForBWT/NASA_access_log_Jul95_converted_sequences.txt 5000 1 99

echo "Finished BWT"
