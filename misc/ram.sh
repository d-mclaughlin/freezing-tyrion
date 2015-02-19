#!/bin/bash

while read line
do
    name=$line
done < $1

pmap $name | grep -o '[0-9]*'| tail -1
#or pmap 41460 | grep total | grep -o '[0-9]*'

