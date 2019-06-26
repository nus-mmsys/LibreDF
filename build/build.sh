#! /usr/bin/env bash

OS=`uname`
OCV=`pkg-config --cflags opencv`
OCV=`echo "$OCV" | cut -d " " -f 1`
OCV=${OCV#-I}
OCV=${OCV%include/opencv}

if [ ${OCV:0:1} != "/" ]
then
	echo "opencv is not installed."
	if [ $OS == "Linux" ]
	then
		echo "apt install libopencv-dev"
	elif [ $OS == "Darwin" ]
	then
		echo "brew install opencv"
	fi
	exit 1
fi

DF_HOME="${HOME}/Documents/df"
if [ ! -d $DF_HOME ]
then
	mkdir $DF_HOME
fi
CASCADE="${DF_HOME}/haarcascades"
if [ ! -d $CASCADE ]
then
	mkdir $CASCADE
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
cp -n ${OCV}/share/opencv/haarcascades/*.xml ${CASCADE}/
if [ $# == 0 ]
then
	echo "Release Build"
	cmake ..
else
	if [ $1 == "debug" ]
	then
		echo "Debug Build"
		cmake -DCMAKE_BUILD_TYPE=DEBUG ..
	else
		cmake $1 -DCUDA_USE_STATIC_CUDA_RUNTIME=OFF ..
	fi
fi
make
