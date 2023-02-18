#!/bin/bash

echo '> echo $(hostname)'
echo $(hostname); 

echo ""

echo '> time ./mrr < inSample.txt'
time ./mrr < inSample.txt; 

echo ""

for f in inMed*.txt
do
    echo "> time ./mrr < $f"
    time ./mrr < $f;
done

