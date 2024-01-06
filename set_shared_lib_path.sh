#!/bin/sh -

########################################################
# A script to set dynamic library paths
########################################################

# Currently it looks like the following 2 shared libs couldn't be found
# Thus set them accordingly
# ----------------------------------------------------------------------
# Pyrad@SSEA MINGW64 /d/Gitee/cpp11
# $ ldd ./build/mymainrun.exe
#         ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x772d0000)
#         kernel32.dll => /c/Windows/system32/kernel32.dll (0x76f70000)
#         KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefcd80000)
#         libcppfeatures.dll => not found
#         libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (?)
#         msvcrt.dll => /c/Windows/system32/msvcrt.dll (?)
#         libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (?)
#         libEffectiveModernCpp.dll => not found
#         libnormal.dll => not found
#         libpyrun.dll => not found

# For now, only the following dynamic libs are needed when loading
lib_0="libcppfeatures.dll"
lib_1="libEffectiveModernCpp.dll"
lib_2="libpyrun.dll"
lib_3="libnormal.dll"
lib_4="libboosttest.dll"
lib_array=($lib_0 $lib_1 $lib_2 $lib_3 $lib_4)

cwd=`pwd`

if [[ ! -z $TERM_PROGRAM ]] && [[ $TERM_PROGRAM == "Tabby" ]] && [[ ! -z $OS ]] && [[ $OS == "Windows_NT" ]] && [[ ! -z $OSTYPE ]] && [[ $OSTYPE == "msys" ]]; then
    # In Windows platform, using msys, and terminal is Tabby
    for ilib in ${lib_array[@]}; do
        ilib_path=`find . -type f -name "$ilib"`
        #echo $ilib_path
        start_dot_slash="./"
        if [[ $ilib_path == "./"* ]]; then
            ilib_path=${ilib_path/#$start_dot_slash/}
            #echo $ilib_path
        fi
        end_fname="/$ilib"
        if [[ $ilib_path == *"$end_fname" ]]; then
            ilib_path=${ilib_path/%$end_fname/}
            #echo $ilib_path
        fi
        cur_lib_path="${cwd}/${ilib_path}"
        echo "Add shared library path: $cur_lib_path"
        export PATH="$PATH:$cur_lib_path"
    done
fi



# Note, though ./mymainrun.exe doesn't depend on libpython38.dll,
# libpyrun.dll now depends on libpython38.dll (2024-01-06)
# So path to libpython38.dll should also be added to env PATH as well!
# If this step is skipped, then the following error would occur,
# which would be very confusing!
# --- --- --- --- --- ---
# Pyrad@SSEA MINGW64 /d/Gitee/cpp11/build
# $ ./mymainrun.exe
# D:/Gitee/cpp11/build/mymainrun.exe: error while loading shared libraries: libpyrun.dll: cannot open shared object file: No such file or directory

#CUR_PYTHON_DYN_LIB_PATH="D:/procs/python38"
CUR_PYTHON_DYN_LIB_PATH="/d/procs/python38"
CUR_PYTHON_DYN_LIB_1="$CUR_PYTHON_DYN_LIB_PATH/python.dll"
CUR_PYTHON_DYN_LIB_2="$CUR_PYTHON_DYN_LIB_PATH/python38.dll"
if [[ -d $CUR_PYTHON_DYN_LIB_PATH ]]; then
	if [[ -f $CUR_PYTHON_DYN_LIB_1 ]] || [[ -f $CUR_PYTHON_DYN_LIB_2 ]]; then
		echo "Add shared library path: $CUR_PYTHON_DYN_LIB_PATH"
		export PATH="$PATH:$CUR_PYTHON_DYN_LIB_PATH"
	else
		echo "[ERROR] Python lib file not found:"
		echo "[ERROR]   $CUR_PYTHON_DYN_LIB_1"
		echo "[ERROR]  or"
		echo "[ERROR]   $CUR_PYTHON_DYN_LIB_2"
	fi
else
	echo "[ERROR] Python lib path not found:"
	echo "[ERROR]  $CUR_PYTHON_DYN_LIB_PATH"
fi
