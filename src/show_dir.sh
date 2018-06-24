#!/bin/bash
PATH=../data/
FILENAME=disk
cd $PATH
for ((i=1;i<=6;i++))
do 
    echo ${FILENAME}$i
#    echo -e "\n"
    /bin/ls -alhk $FILENAME$i
#    echo -e "\n"
done;


