#! /usr/bin/env bash

OS=`uname`

DF_HOME="${HOME}/Documents/df"
if [ ! -d $DF_HOME ]
then
	mkdir $DF_HOME
fi
OUTPUTS="${DF_HOME}/outputs"
if [ ! -d $OUTPUTS ]
then
	mkdir $OUTPUTS
fi
LOGS="${DF_HOME}/logs"
if [ ! -d $LOGS ]
then
	mkdir $LOGS
fi
echo "Release Build"
cmake -DBUILD_RDF_MODULE=ON -DBUILD_ML_ACTORS=OFF -DBUILD_OPENCV_ACTORS=OFF -DBUILD_COMBINATORICS_ACTORS=OFF -DBUILD_DESIGN_ACTORS=OFF -DBUILD_SIGNAL_ACTORS=OFF ..
make
