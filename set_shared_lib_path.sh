#!/bin/sh -

########################################################
# A script to set dynamic library paths
########################################################

# Currently it looks like the following 2 shared libs couldn't be found
# Thus set them accordingly
# ----------------------------------------------------------------------
# Pyrad@SSEA MINGW64 /d/Gitee/cpp11
# $ ldd ./build/mymainrun.exe
#         ntdll.dll => /c/Windows/SYSTEM32/ntdll.dll (0x76ed0000)
#         kernel32.dll => /c/Windows/system32/kernel32.dll (0x76cb0000)
#         KERNELBASE.dll => /c/Windows/system32/KERNELBASE.dll (0x7fefcaf0000)
#         libcppfeatures.dll => not found
#         libgcc_s_seh-1.dll => /mingw64/bin/libgcc_s_seh-1.dll (?)
#         msvcrt.dll => /c/Windows/system32/msvcrt.dll (?)
#         libstdc++-6.dll => /mingw64/bin/libstdc++-6.dll (?)
#         libEffectiveModernCpp.dll => not found


# For now, only 2 libs are needed when loading
lib_0="libcppfeatures.dll"
lib_1="libEffectiveModernCpp.dll"
lib_array=($lib_0 $lib_1)

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
