#!/bin/bash


for i in {5..5}
do
for j in {4..6}
do
Clist=(2 3 5 10 15 20 30 40 50)
for C in ${Clist[@]}
do
let SCALE=10**$i   # sets SCALE to 10Ei.
VERTICES=$(($j*$SCALE))
EDGES=$(($VERTICES*$C))
filename="${VERTICES}_${EDGES}_${C}_RMAT.txt"
filenameShifted="${filename}_start_from_1.txt"
filenameMTX="${VERTICES}_${EDGES}_${C}_RMAT.txt.mtx"
../PaRMAT/Release/PaRMAT -nVertices ${VERTICES} -nEdges ${EDGES} -output $filename -threads 8 -sorted -noEdgeToSelf -noDuplicateEdges -undirected -a 0.45 -b 0.15 -c 0.15 -d 0.25
#../PaRMAT/Release/PaRMAT -nVertices ${VERTICES} -nEdges ${EDGES} -output $filename -threads 16 -sorted -noEdgeToSelf -noDuplicateEdges -undirected -a 0.25 -b 0.25 -c 0.25 -d 0.25

awk -v s=1 '{print $1+s, $2+s}' $filename > $filenameShifted

cp $filenameShifted $filenameMTX
rm $filename
rm $filenameShifted

sed -i "1s/^/%%MatrixMarket matrix coordinate pattern symmetric\n/" $filenameMTX
sed -i "2s/^/$VERTICES $VERTICES $EDGES\n/" $filenameMTX

~/Matching/build/matcher $filenameMTX 5 1 0
~/Matching/build/matcher $filenameMTX 5 2 1
~/Matching/build/matcher $filenameMTX 5 4 1
~/Matching/build/matcher $filenameMTX 5 8 1
#rm $filenameMTX


done
done
done
