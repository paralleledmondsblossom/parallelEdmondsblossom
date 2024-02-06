#!/bin/bash

C=10
for i in {6..6}
do
for deg in 2 3 4 5 6 7 8 9 10
do
for j in {4..10}
do
let SCALE=10**$i   # sets SCALE to 10Ei.
VERTICES=$(($j*$SCALE))
filenameKece="${VERTICES}_${deg}_REG_RAND.txt"
filenameMTX="${filenameKece}.mtx"
python generate_random_graph.py $VERTICES $deg $filenameKece matrix_market
#./generate ${VERTICES} ${deg} ${filenameMTX}
#../../src/matching $filenameKece
~/Matching/build/matcher $filenameMTX 5 1 0
~/Matching/build/matcher $filenameMTX 5 2 1
~/Matching/build/matcher $filenameMTX 5 4 1
~/Matching/build/matcher $filenameMTX 5 8 1
#/home/greg/mvm/src/cpu $filenameMTX
#/home/greg/mvm/src/a.out $filenameMTX
#/home/greg/mvm/src/a.out $filenameMTX 60 48
#rm $filename
done
done
done
