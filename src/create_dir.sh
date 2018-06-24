#!/bin/bash
PATH=../data/
FILENAME=disk
cd $PATH

for ((i=1;i<=6;i++))
do 
    /bin/mkdir -p $FILENAME$i
    echo $(pwd)
done;

