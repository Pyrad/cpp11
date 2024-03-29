#!/bin/bash -

[[ -z "$BOOST_INC_HOME" || ! -d "$BOOST_INC_HOME" ]] && echo "Env var BOOST_INC_HOME is not found" && exit 20
[[ -z "$BOOST_LIB_HOME" || ! -d "$BOOST_LIB_HOME" ]] && echo "Env var BOOST_LIB_HOME is not found" && exit 20
echo "Found env var BOOST_INC_HOME: $BOOST_INC_HOME"
echo "Found env var BOOST_LIB_HOME: $BOOST_LIB_HOME"

MKF_TMPL="./template.makefile"
TAR_MAKEFILE="GNUmakefile"

[[ ! -f $MKF_TMPL ]] && echo "Template file($MKF_TMPL) for Makefile is NOT found" && exit 2
echo "Template file($MKF_TMPL) for Makefile is found"
[[ -f $TAR_MAKEFILE ]] && echo "Warning: removing old $TAR_MAKEFILE" && rm -rf $TAR_MAKEFILE

CXX_BIN=`which g++`
[[ $? == 1 ]] && echo "Error: g++ is not found" && exit 2
echo "g++ is $CXX_BIN"

#GXX_VER=`g++ --version | head -n 1 | cut -d' ' -f 3`
GXX_VER_STR=`g++ --version | head -n 1`
GXX_VER_STR_LIST=($GXX_VER_STR)
VERSTR_IDX=`expr ${#GXX_VER_STR_LIST[@]} - 1`
GXX_VER=${GXX_VER_STR_LIST[$VERSTR_IDX]}
echo "g++ version is: ${GXX_VER}"

UTILS_FILE="utils.config.sh"
[[ ! -f $UTILS_FILE ]] && echo "$UTILS_FILE not found" && exit 2
echo "Searching for boost library string..."
. $UTILS_FILE
BVERSTR=`find_boost_version_str $BOOST_LIB_HOME`
[[ -z $BVERSTR ]] && echo "Empty string found for boost library"
echo "Boost library string found: $BVERSTR"
[[ $? -ne 0 ]] && echo "Error source $UTILS_FILE" && exit 2

cp $MKF_TMPL $TAR_MAKEFILE

### Don't use '/' in reg expression in sed, as $BOOST_INC_HOME has '/'
### in its string value
sed -i "s#<__BOOST_INC_TO_FILL__>#${BOOST_INC_HOME}#g" $TAR_MAKEFILE
sed -i "s#<__BOOST_LIB_TO_FILL__>#${BOOST_LIB_HOME}#g" $TAR_MAKEFILE
sed -i "s#<__CXX_TO_FILL>#${CXX_BIN}#g" $TAR_MAKEFILE
# Replace boost library version string
sed -i "s#<__BVSTR__>#${BVERSTR}#g" $TAR_MAKEFILE


### Clean old objects & binary directory
OBJ_DIR="./objs"
BIN_DIR="./bin"
if [[ -d $OBJ_DIR ]]; then
    echo "Warning: old objects in dir <$OBJ_DIR> will be removed"
    rm -rf ${OBJ_DIR}/*
else
    echo "Creating directory for objects: $OBJ_DIR"
    mkdir $OBJ_DIR
fi

if [[ -d $BIN_DIR ]]; then
    echo "Warning: old binaries in dir <$BIN_DIR> will be removed"
    rm -rf ${BIN_DIR}/*
else
    echo "Creating directory for objects: $BIN_DIR"
    mkdir $BIN_DIR
fi

echo "Target make file created: $TAR_MAKEFILE"
