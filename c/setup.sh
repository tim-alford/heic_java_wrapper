#!/usr/bin/bash
if [ -z "$JAVA_HOME" ]; then
	echo "Please set JAVA_HOME before running this script."
	exit 1
fi
export C_DIR=$PWD
export HEIF_INCLUDE_DIR=/usr/local/include/libheif
export JAVA_DIR=./../java
export JAVA_GENERATED=$(find $JAVA_DIR -name "generated" )
export WRAPPER_HEADERS=$JAVA_GENERATED/sources/headers/java/main
export JAVA_HEADERS=$JAVA_HOME/include
export JAVA_LINUX_HEADERS=$JAVA_HEADERS/linux
