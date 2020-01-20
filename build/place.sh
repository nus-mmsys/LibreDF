#! /usr/bin/env bash

if [ $# == 0 ]
then
	echo "usage:   place.sh <processors> <input file>"
	echo "example: place.sh 0,1,2 test.rdf"
elif [ $# == 1 ]
then
	./rdf $1
elif [ $# == 2 ]
then
	taskset -c $1 ./rdf $2
fi
