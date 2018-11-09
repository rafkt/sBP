#!/bin/bash
clear
cd imgages/
rm -rf *.jpeg ../cacheStats.pdf
cd ..
Rscript rScatterPlot.r
cd imgagesStats/
convert `ls | sort -n` ../cacheStats.pdf
cd ..


