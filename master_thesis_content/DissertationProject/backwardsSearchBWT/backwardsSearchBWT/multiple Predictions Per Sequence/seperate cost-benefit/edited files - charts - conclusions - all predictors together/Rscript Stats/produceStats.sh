#!/bin/bash
clear
cd imgagesStatsCache/
rm -rf *.jpeg ../cacheStatsCache.pdf
cd ..
Rscript multiplePredictionsPerSequence.r
cd imgagesStatsCache/
convert `ls | sort -n` ../cacheStats.pdf
cd ..


cd imgagesStatsNoCache/
rm -rf *.jpeg ../noCacheStatsCache.pdf
cd ..
Rscript multiplePredictionsPerSequence_NOCACHE.r
cd imgagesStatsNoCache/
convert `ls | sort -n` ../noCacheStats.pdf
cd ..

