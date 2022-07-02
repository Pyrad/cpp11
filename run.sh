#!/bin/sh -

BDIR="bin"
BINARY_FILE_LINUX="${BDIR}/run-g"
BINARY_FILE_WINDOWS="${BDIR}/run-g.exe"

BIN_FILE_RUN=""
if [[ -f $BINARY_FILE_LINUX ]]; then
	BIN_FILE_RUN=$BINARY_FILE_LINUX
else
	BIN_FILE_RUN=$BINARY_FILE_WINDOWS
fi

[[ ! -f $BIN_FILE_RUN ]] && echo "No binary found" && exit 2

time $BIN_FILE_RUN



