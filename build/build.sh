#! /usr/bin/env bash
OCV=`pkg-config --libs opencv`
if [ "$OCV" == "" ]
then
	echo "opencv is not installed."
	exit 1
fi
OCV=`echo "$OCV" | cut -d " " -f 1`
OCV=${OCV#-L}
OCV=${OCV%lib}
DF_HOME="${HOME}/Documents/df"
if [ ! -d $DF_HOME ]
then
	mkdir $DF_HOME
fi
CASCADE="${DF_HOME}/haarcascades"
if [ ! -d ${CASCADE} ]
then
	mkdir ${CASCADE}
fi
OUTPUTS="${DF_HOME}/outputs"
if [ ! -d ${OUTPUTS} ]
then
	mkdir ${OUTPUTS}
fi
cp -n ${OCV}/share/OpenCV/haarcascades/*.xml ${CASCADE}/
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
		cmake $1 ..
	fi
fi
make
