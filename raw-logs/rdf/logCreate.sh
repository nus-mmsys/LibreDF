#! /usr/bin/env bash

if [ $# == 0 ]
then
	echo "usage: logCreate.sh <log name>"
else
	cat ~/Documents/df/logs/Src ~/Documents/df/logs/Snk > $1 
fi
