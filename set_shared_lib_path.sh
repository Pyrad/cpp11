#!/bin/sh -

########################################################
# A script to set dynamic library paths
########################################################

# I add prefix "S_" to make these variables unique, because this script would be sourced,
# so it's possible some names have already been used/set, so a prefix might help to reduce
# the possibility of conflict.

# Variables set in this script, which need to be unset after this script is sourced
S_MY_BOOST_HOME_DIR=
S_BOOST_INCLUDEDIR=
S_BOOST_LIBRARYDIR=
S_MY_PYTHON3_HOME_DIR=
S_Python3_INCLUDE_DIRS=
S_Python3_LIBRARIES=
S_Python3_LIBRARY=
S_PYTHON_EXECUTABLE=
S_readme_fname="README.md"
S_src_dir_name="src"
S_build_dir_name="build"
S_cur_wd=`pwd`
S_ilib=
S_ilib_path=
S_end_fname=
S_cur_lib_path=
S_CUR_PYTHON_DYN_LIB_PATH=
S_CUR_PYTHON_DYN_LIB_1=
S_CUR_PYTHON_DYN_LIB_2=
S_LINK_LIB_PATH_ARRAY=
S_LINK_LIB_PATH_ARRAY_FINAL=

# Array variables which are also needed to be unset at last
declare -a S_lib_array
declare -a S_to_append_arr
declare -a S_temp_lib_arr
S_lib_array=
S_to_append_arr=
S_temp_lib_arr=




if [[ -z $DEV_NICKNAME ]]; then
	echo "Environment variable DEV_NICKNAME must be set to one of the "
	echo "followings according to different machines platforms"
	echo "  (1) Asus_Win7_MSYS2"
	echo "  (2) Asus_Win7_VBox_OpenSUSE"
	echo "  (3) Asus_Win7_VBox_Ubuntu_20_04_LTS"
	return 22
fi

if [[ $DEV_NICKNAME != "Asus_Win7_MSYS2" ]] && [[ $DEV_NICKNAME != "Asus_Win7_VBox_OpenSUSE" ]] && [[ $DEV_NICKNAME != "Asus_Win7_VBox_Ubuntu_20_04_LTS" ]]; then
	echo "Environment variable DEV_NICKNAME must be one of the "
	echo "followings according to different machines platforms"
	echo "  (1) Asus_Win7_MSYS2"
	echo "  (2) Asus_Win7_VBox_OpenSUSE"
	echo "  (3) Asus_Win7_VBox_Ubuntu_20_04_LTS"
	echo "Error: DEV_NICKNAME found is: $DEV_NICKNAME"
	return 22
fi

# Set Python3 & boost lib paths according to different machines
if [[ $DEV_NICKNAME == "Asus_Win7_VBox_Ubuntu_20_04_LTS" ]]; then
    S_MY_BOOST_HOME_DIR="/home/pyrad/procs/boost_1_73_0"
    S_BOOST_INCLUDEDIR="${S_MY_BOOST_HOME_DIR}/include"
    S_BOOST_LIBRARYDIR="${S_MY_BOOST_HOME_DIR}/lib"
	S_MY_PYTHON3_HOME_DIR="/home/pyrad/procs/Python-3.8.3"
    S_Python3_INCLUDE_DIRS="${S_MY_PYTHON3_HOME_DIR}/include/python3.8"
    S_Python3_LIBRARIES="${S_MY_PYTHON3_HOME_DIR}/lib/libpython3.8.so"
    S_Python3_LIBRARY="${S_MY_PYTHON3_HOME_DIR}/lib"
    S_PYTHON_EXECUTABLE="${S_MY_PYTHON3_HOME_DIR}/bin/python3"
	S_CUR_PYTHON_DYN_LIB_PATH="${S_MY_PYTHON3_HOME_DIR}/lib"
	S_CUR_PYTHON_DYN_LIB_1="${S_CUR_PYTHON_DYN_LIB_PATH}/libpython3.8.so"
	S_CUR_PYTHON_DYN_LIB_2="${S_CUR_PYTHON_DYN_LIB_PATH}/libpython3.so"
elif [[ $DEV_NICKNAME == "Asus_Win7_MSYS2" ]]; then
    #set(S_MY_BOOST_HOME_DIR "D:/procs/boost_1_79_0")
    # (2023-10-04) Start to use boost_1_83_0 from today on
    S_MY_BOOST_HOME_DIR="/d/procs/boost_1_83_0"
    S_BOOST_INCLUDEDIR="${S_MY_BOOST_HOME_DIR}/include/boost-1_83"
    S_BOOST_LIBRARYDIR="${S_MY_BOOST_HOME_DIR}/lib"
	S_MY_PYTHON3_HOME_DIR="/d/procs/python38"
    S_Python3_INCLUDE_DIRS="${S_MY_PYTHON3_HOME_DIR}/include"
    S_Python3_LIBRARIES="${S_MY_PYTHON3_HOME_DIR}/python38.dll"
    S_Python3_LIBRARY="${S_MY_PYTHON3_HOME_DIR}"
    S_PYTHON_EXECUTABLE="${S_MY_PYTHON3_HOME_DIR}/python.exe"
	S_CUR_PYTHON_DYN_LIB_PATH="${S_MY_PYTHON3_HOME_DIR}"
	S_CUR_PYTHON_DYN_LIB_1="$S_CUR_PYTHON_DYN_LIB_PATH/python.dll"
	S_CUR_PYTHON_DYN_LIB_2="$S_CUR_PYTHON_DYN_LIB_PATH/python38.dll"
elif [[ $DEV_NICKNAME == "Asus_Win7_VBox_OpenSUSE" ]]; then
    echo "Error: Python3 lib & boost lib paths have not been set for $DEV_NICKNAME yet"
	return 22
else
	echo "Unexpected error."
	return 22
fi


# Check if current path is the root directory of cpp11 git repo
if [[ ! -f $S_readme_fname ]] || [[ ! -d $S_src_dir_name ]]; then
	echo "ERROR: File $S_readme_fname and $S_src_dir_name are not found"
	echo "ERROR: Current directory might not be the cpp11 git repo root directory"
	echo "ERROR: Exit."
	return 2
fi

# This script is only allowed to be sourced. So don't continue if not sourced
if [[ $0 == $BASH_SOURCE ]]; then
	echo "ERROR: This script need to be sourced other than run in a sub-shell"
	return 22
fi

# Build directory must exist
if [[ ! -d $S_build_dir_name ]]; then
	echo "ERROR: Build directory does not exist"
	return 20
fi

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
S_lib_array=("libcppfeatures.dll" "libEffectiveModernCpp.dll" "libpyrun.dll" "libnormal.dll" "libboosttest.dll")

if [[ ! -z $TERM_PROGRAM ]] && [[ $TERM_PROGRAM == "Tabby" ]] && [[ ! -z $OS ]] && [[ $OS == "Windows_NT" ]] && [[ ! -z $OSTYPE ]] && [[ $OSTYPE == "msys" ]]; then
    # In Windows platform, using msys, and terminal is Tabby
    for S_ilib in ${S_lib_array[@]}; do
        S_ilib_path=`find "$S_build_dir_name" -type f -name "$S_ilib"`
		[[ -z $S_ilib_path ]] && continue

        start_dot_slash="./"
        if [[ $S_ilib_path == "./"* ]]; then
            S_ilib_path=${S_ilib_path/#$start_dot_slash/}
        fi
        S_end_fname="/$S_ilib"
        if [[ $S_ilib_path == *"$S_end_fname" ]]; then
            S_ilib_path=${S_ilib_path/%$S_end_fname/}
        fi
        S_cur_lib_path="${S_cur_wd}/${S_ilib_path}"
        echo "Found shared library path: $S_cur_lib_path ($S_ilib)"
        #export PATH="$PATH:$S_cur_lib_path"
		[[ -z $S_LINK_LIB_PATH_ARRAY ]] && S_LINK_LIB_PATH_ARRAY="$S_cur_lib_path" || S_LINK_LIB_PATH_ARRAY="$S_LINK_LIB_PATH_ARRAY:$S_cur_lib_path"
    done
else
	echo "ERROR: Not a targeted machine, please check"
	return
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

if [[ -d $S_CUR_PYTHON_DYN_LIB_PATH ]]; then
	if [[ -f $S_CUR_PYTHON_DYN_LIB_1 ]] || [[ -f $S_CUR_PYTHON_DYN_LIB_2 ]]; then
		echo "Found shared library path: $S_CUR_PYTHON_DYN_LIB_PATH"
		#export PATH="$PATH:$S_CUR_PYTHON_DYN_LIB_PATH"
		[[ -z $S_LINK_LIB_PATH_ARRAY ]] && S_LINK_LIB_PATH_ARRAY="$S_CUR_PYTHON_DYN_LIB_PATH" || S_LINK_LIB_PATH_ARRAY="$S_LINK_LIB_PATH_ARRAY:$S_CUR_PYTHON_DYN_LIB_PATH"
	else
		echo "[ERROR] Python lib file not found:"
		echo "[ERROR]   $S_CUR_PYTHON_DYN_LIB_1"
		echo "[ERROR]  or"
		echo "[ERROR]   $S_CUR_PYTHON_DYN_LIB_2"
		return 2
	fi
else
	echo "[ERROR] Python lib path not found:"
	echo "[ERROR]  $S_CUR_PYTHON_DYN_LIB_PATH"
	return 2
fi

echo "Lib paths collected to append:"
echo $S_LINK_LIB_PATH_ARRAY | awk -F: '{for (i=0;++i<=NF;) print "  ", $i}'

# All lib candidates to append to the final PATH or LD_LIBRARY_PATH variable
S_temp_lib_arr=$(echo $S_LINK_LIB_PATH_ARRAY | tr ':' ' ')
#
if [[ ! -z $OS ]] && [[ $OS == "Windows_NT" ]]; then
	S_to_append_arr=$(echo $PATH | tr ':' ' ')
else
	S_to_append_arr=$(echo $LD_LIBRARY_PATH | tr ':' ' ')
fi

temp_var=
declare -i temp_found
temp_found=0
for S_ilib in ${S_temp_lib_arr[@]}; do
	for temp_var in ${S_to_append_arr[@]}; do
		if [[ $temp_var == $S_ilib ]]; then
			temp_found=1
			break
		fi
	done
	if [[ $temp_found -eq 1 ]]; then
		echo "Skip a lib path already appended: $S_ilib"
	else
		[[ -z $S_LINK_LIB_PATH_ARRAY_FINAL ]] && S_LINK_LIB_PATH_ARRAY_FINAL="$S_ilib" || S_LINK_LIB_PATH_ARRAY_FINAL="${S_LINK_LIB_PATH_ARRAY_FINAL}:${S_ilib}"
	fi
	temp_found=0
done
unset temp_var
unset temp_found

# Append final lib paths to the targeted env variable (PATH or LD_LIBRARY_PATH)
if [[ -z $S_LINK_LIB_PATH_ARRAY_FINAL ]]; then
	echo "No lib path to append to PATH/LD_LIBRARY_PATH, skip."
else
	echo "Final lib paths to append:"
	echo $S_LINK_LIB_PATH_ARRAY_FINAL | awk -F: '{for (i=0;++i<=NF;) print "  ", $i}'
	if [[ ! -z $OS ]] && [[ $OS == "Windows_NT" ]]; then
		export PATH="$PATH:$S_LINK_LIB_PATH_ARRAY_FINAL"
		echo "Added to PATH"
	else
		export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$S_LINK_LIB_PATH_ARRAY_FINAL"
		echo "Added to LD_LIBRARY_PATH"
	fi
fi

unset S_MY_BOOST_HOME_DIR
unset S_BOOST_INCLUDEDIR
unset S_BOOST_LIBRARYDIR
unset S_MY_PYTHON3_HOME_DIR
unset S_Python3_INCLUDE_DIRS
unset S_Python3_LIBRARIES
unset S_Python3_LIBRARY
unset S_PYTHON_EXECUTABLE
unset S_readme_fname
unset S_src_dir_name
unset S_cur_wd
unset S_ilib
unset S_ilib_path
unset S_end_fname
unset S_cur_lib_path
unset S_CUR_PYTHON_DYN_LIB_PATH
unset S_CUR_PYTHON_DYN_LIB_1
unset S_CUR_PYTHON_DYN_LIB_2
unset S_lib_array
unset S_LINK_LIB_PATH_ARRAY
unset S_LINK_LIB_PATH_ARRAY_FINAL
unset S_to_append_arr
unset S_temp_lib_arr



