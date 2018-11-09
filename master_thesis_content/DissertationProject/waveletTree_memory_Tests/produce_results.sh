#!/bin/bash
clear
g++ -g -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib wt_comparisons.cpp -o program -lsdsl -ldivsufsort -ldivsufsort64
./program BIBLE.txt_sequences.txt
./program BMS1_spmf.txt_sequences.txt
./program FIFA.txt_sequences.txt
./program Kosarak_converted.txt_sequences.txt
./program LEVIATHAN.txt_sequences.txt
./program MSNBC.txt_sequences.txt
./program SIGN.txt_sequences.txt