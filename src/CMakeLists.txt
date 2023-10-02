#############################################################################################
### How to make this project ?
###
###  $ cmake -G Ninja <path-to-this-cmake-file> -DCMAKE_BUILD_TYPE=Release -DBoost_DEBUG=ON
###  $ cmake --build .
###
#############################################################################################


MESSAGE(STATUS "[PYRAD] ============= ============= ============= ==============") 
MESSAGE(STATUS "[PYRAD] ============= [Main CMakeLists.txt starts] =============") 
MESSAGE(STATUS "[PYRAD] ============= ============= ============= ==============") 

# Set the minimun version of CMake
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

# Set the project name
PROJECT(MYCPP11TEST VERSION 0.1)

# specify the C++ standard
#SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

# If you'd like to build in a Unix way in windows platform,
# add the following
# SET(MY_MINGW64_HOME "/mingw64")
# SET(CMAKE_MAKE_PROGRAM "${MY_MINGW64_HOME}/bin/mingw32-make.exe")
# SET(CMAKE_C_COMPILER "${MY_MINGW64_HOME}/bin/gcc.exe")
# SET(CMAKE_CXX_COMPILER "${MY_MINGW64_HOME}/bin/g++.exe")

MESSAGE(STATUS "[PYRAD] This is the BINARY directory = " ${MYCPP11TEST_BINARY_DIR})
MESSAGE(STATUS "[PYRAD] This is the SOURCE directory = " ${MYCPP11TEST_SOURCE_DIR})
MESSAGE(STATUS "[PYRAD]                       CYGWIN = " ${CYGWIN})
MESSAGE(STATUS "[PYRAD]                        MINGW = " ${MINGW})
MESSAGE(STATUS "[PYRAD]                         MSYS = " ${MSYS})
MESSAGE(STATUS "[PYRAD]                         UNIX = " ${UNIX})
MESSAGE(STATUS "[PYRAD]                 CMAKE_SYSTEM = " ${CMAKE_SYSTEM})
MESSAGE(STATUS "[PYRAD]            CMAKE_SYSTEM_NAME = " ${CMAKE_SYSTEM_NAME})
MESSAGE(STATUS "[PYRAD]       CMAKE_SYSTEM_PROCESSOR = " ${CMAKE_SYSTEM_PROCESSOR})
MESSAGE(STATUS "[PYRAD]  CMAKE_HOST_SYSTEM_PROCESSOR = " ${CMAKE_HOST_SYSTEM_PROCESSOR})
MESSAGE(STATUS "[PYRAD]            CMAKE_HOST_SYSTEM = " ${CMAKE_HOST_SYSTEM})
MESSAGE(STATUS "[PYRAD]       CMAKE_HOST_SYSTEM_NAME = " ${CMAKE_HOST_SYSTEM_NAME})
MESSAGE(STATUS "[PYRAD]           PROJECT_BINARY_DIR = " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "[PYRAD]           PROJECT_SOURCE_DIR = " ${PROJECT_SOURCE_DIR})

# A portable way to check if 32-bit or 64-bit platform
# If it is 4, the platform is 32-bit; if it is 8, the platform is 64-bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(bstring "64-bit platform")
else()
    SET(bstring "32-bit platform")
endif()
MESSAGE(STATUS "[PYRAD]          CMAKE_SIZEOF_VOID_P = ${CMAKE_SIZEOF_VOID_P} (${bstring})")

# ### Print out some host information
# ### Looks nothing got on Windows?
# MESSAGE(STATUS "[PYRAD]")
# MESSAGE(STATUS "[PYRAD] ===== Some info got by cmake_host_system_information =====")
# cmake_host_system_information(RESULT PRETTY_NAME QUERY DISTRIB_PRETTY_NAME)
# MESSAGE(STATUS "[PYRAD] PRETTY_NAME: ${PRETTY_NAME}")
# cmake_host_system_information(RESULT DISTRO QUERY DISTRIB_INFO)
# foreach(VAR IN LISTS DISTRO)
#     MESSAGE(STATUS "[PYRAD]   [DISTRO] ${VAR} = `${${VAR}}`")
# endforeach()

cmake_host_system_information(RESULT val_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
MESSAGE(STATUS "[PYRAD] NUMBER_OF_LOGICAL_CORES: ${val_NUMBER_OF_LOGICAL_CORES}")

# Create a list of keys for query host system information
set(QueryKeyList NUMBER_OF_PHYSICAL_CORES HOSTNAME FQDN TOTAL_VIRTUAL_MEMORY
                 AVAILABLE_VIRTUAL_MEMORY TOTAL_PHYSICAL_MEMORY AVAILABLE_PHYSICAL_MEMORY
                 IS_64BIT HAS_FPU HAS_MMX HAS_MMX_PLUS HAS_SSE HAS_SSE2 HAS_SSE_FP
                 HAS_SSE_MMX HAS_AMD_3DNOW HAS_AMD_3DNOW_PLUS HAS_IA64 HAS_SERIAL_NUMBER
                 PROCESSOR_SERIAL_NUMBER PROCESSOR_NAME PROCESSOR_DESCRIPTION OS_NAME
                 OS_RELEASE OS_VERSION OS_PLATFORM DISTRIB_INFO)
# Query each key and then print the information about the system
# LIST(LENGTH QueryKeyList klen)
# MESSAGE(STATUS "[PYRAD] Query key list length: ${klen}")
# foreach( cur_val ${QueryKeyList})
#     CMAKE_HOST_SYSTEM_INFORMATION(RESULT qval QUERY ${cur_val})
#     MESSAGE(STATUS "[PYRAD] [QueryKeyList]  ${cur_val} : ${qval}")
# endforeach()

MESSAGE(STATUS "[PYRAD] ==========================================================")



# In order to compiled with boost successfully, the boost library
# should be compiled with the following command,
# ==============================================================
# .\b2.exe install \
#		--build-type=complete \
#		threading=multi \
#		link=shared \
#		address-model=64 \
#		toolset=gcc \
#		--prefix="D:\procs\boost_1_79_0"
#
# The structure of directories should look like below,
# ==============================================================
# boost_1_79_0_system/
#	|---include/
#	|		|---boost-1_79/
#	|		      |---boost/
#	|		      	  |---<all *.hpp headers>
#	|		      	  |---<all_header_filer_folders>
#	|
#	|---lib/
#		 |---cmake/
#		 |---<all *.dll.a files>
#		 |---<all *.dll files>

################################################################################
# Environmental variable DISTINGUISHED_NICKNAME must be defined to check which
# platform we are on, thus we could decide which boost version to use and where
# to find it.
#
# Following variables will be set accoording to the platform:
#   (1) MY_BOOST_HOME_DIR
#   (2) Python3_INCLUDE_DIRS
#   (3) Python3_LIBRARIES
#   (4) Python3_LIBRARY
#   (5) PYTHON_EXECUTABLE
#
# For variables Python3_INCLUDE_DIRS, Python3_LIBRARIES, Python3_LIBRARY and
# PYTHON_EXECUTABLE, they can be found in https://cmake.org/cmake/help/latest/module/FindPython3.html
################################################################################
if (NOT DEFINED ENV{DISTINGUISHED_NICKNAME})
    MESSAGE(FATAL_ERROR "[PYRAD] DISTINGUISHED_NICKNAME must be set to distinguish host machines!")
endif()
MESSAGE(STATUS "[PYRAD] A customized env variable DISTINGUISHED_NICKNAME is defined to distinguish host machines!")
MESSAGE(STATUS "[PYRAD] DISTINGUISHED_NICKNAME = $ENV{DISTINGUISHED_NICKNAME}")
if ($ENV{DISTINGUISHED_NICKNAME} STREQUAL "Asus_Win7_VBox_Ubuntu_20_04_LTS")
    set(MY_BOOST_HOME_DIR "/home/pyrad/procs/boost_1_73_0")
    set(Python3_INCLUDE_DIRS "/home/pyrad/procs/Python-3.8.3/include/python3.8")
    set(Python3_LIBRARIES "/home/pyrad/procs/Python-3.8.3/lib/libpython3.8.so")
    set(Python3_LIBRARY "/home/pyrad/procs/Python-3.8.3/lib")
    set(PYTHON_EXECUTABLE "/home/pyrad/procs/Python-3.8.3/bin/python3")
elseif ($ENV{DISTINGUISHED_NICKNAME} STREQUAL "Asus_Win7_MSYS2")
    set(MY_BOOST_HOME_DIR "D:/procs/boost_1_79_0")
    set(Python3_INCLUDE_DIRS "D:/procs/python38/include")
    set(Python3_LIBRARIES "D:/procs/python38/python38.dll")
    set(Python3_LIBRARY "D:/procs/python38")
    set(PYTHON_EXECUTABLE "D:/procs/python38/python.exe")
else()
    MESSAGE(STATUS "[PYRAD] Currently (2023-10-02) DISTINGUISHED_NICKNAME must be the following.")
    MESSAGE(STATUS "[PYRAD]   (1) Asus_Win7_MSYS2")
    MESSAGE(STATUS "[PYRAD]   (2) Asus_Win7_VBox_OpenSUSE")
    MESSAGE(STATUS "[PYRAD]   (3) Asus_Win7_VBox_Ubuntu_20_04_LTS")
    MESSAGE(FATAL_ERROR "[PYRAD] Unknown system, because DISTINGUISHED_NICKNAME is not one of the above values.")
endif()
MESSAGE(STATUS "[PYRAD] MY_BOOST_HOME_DIR is set to: ${MY_BOOST_HOME_DIR}")

# In case environmental variable MINGW is not defined on Linux platform
if (DEFINED MINGW)
	MESSAGE(STATUS "[PYRAD] MinGW variable defined!")
else()
	MESSAGE(STATUS "[PYRAD] MinGW variable not defined!")
    set(MINGW 0)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" AND ${MINGW} EQUAL 1)
	MESSAGE(STATUS "[PYRAD] Windows MinGW system!")
	MESSAGE(STATUS "[PYRAD] Assume current is MSYS2")
    #set(MY_BOOST_HOME_DIR "D:/procs/boost_1_79_0")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    MESSAGE(STATUS "[PYRAD] Linux system!")
    #set(MY_BOOST_HOME_DIR /home/pyrad/procs/boost_1_73_0)
elseif(${UNIX} STREQUAL "Unix")
	MESSAGE(STATUS "[PYRAD] Unix system!")
    #set(MY_BOOST_HOME_DIR /home/pyrad/procs/boost_1_73_0)
else()
    MESSAGE(FATAL_ERROR "[PYRAD] Unknown system, can't compile, exit!")
endif()

set(BOOST_ROOT ${MY_BOOST_HOME_DIR})
set(BOOST_INCLUDEDIR ${MY_BOOST_HOME_DIR}/include)
set(BOOST_LIBRARYDIR ${MY_BOOST_HOME_DIR}/lib)
set(Boost_USE_STATIC_LIBS OFF)

# #set(Boost_LIB_PREFIX "lib")
# #set(Boost_USE_STATIC_LIBS ON)
# #set(Boost_USE_STATIC_RUNTIME ON)

# Try to analyze boost library and look for headers & libs required
find_package(Boost 1.65.0 REQUIRED COMPONENTS filesystem regex)

#check if boost was found
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "[PYRAD] boost library is found")
	message(STATUS "[PYRAD] Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    message(STATUS "[PYRAD] Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    message(STATUS "[PYRAD] Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
else()
    message (FATAL_ERROR "Cannot find Boost")
endif()

# Create a list to append libraries for linking
set(LIB_LIST "")

# Create a list to append libraries for headers to include
set(HEADER_LIST "")

# Define a function to append variables
#function(F_APPEND_VAL_TO_2_LIST val alist blist)
#    list(APPEND ${alist} ${val})
#    list(APPEND ${blist} ${val})
#endfunction(F_APPEND_VAL)

# Add the utilites library at all time
set(LIB_UTILITIES "utilities")
add_subdirectory(${LIB_UTILITIES})
list(APPEND LIB_LIST ${LIB_UTILITIES})

###############################################################################
# Define a few macros, these macro can be set at commad line when building
###############################################################################
############################
# Macro 1: ENB_NORMAL
############################
# Define a macro to enable testing for normal C++ functionalities testing (src/normal)
option(ENB_NORMAL "Testing c++ functionalities" ON)
# add the normal library on condition that ENB_NORMAL is turned ON
set(LIB_NORMAL "normal")
if(ENB_NORMAL)
    add_subdirectory(${LIB_NORMAL})
    list(APPEND LIB_LIST ${LIB_NORMAL})
endif()

############################
# Macro 2: ENB_BOOSTTEST
############################
# Define a macro to enable testing for boost testing (src/boosttest)
option(ENB_BOOSTTEST "Testing boost functionalities" ON)
# add the boosttest library on condition that ENB_BOOSTTEST is turned ON
set(LIB_BOOSTTEST "boosttest")
if(ENB_BOOSTTEST)
    add_subdirectory(${LIB_BOOSTTEST})
    list(APPEND LIB_LIST ${LIB_BOOSTTEST})
endif()

############################
# Macro 3: ENB_CPP_FEATURES
############################
# Define a macro to enable testing for CPP feature testing (src/cppfeatures)
option(ENB_CPP_FEATURES "Testing cpp features" ON)
# add the Cpp features library at all time
set(LIB_CPPFEATURES "cppfeatures")
if(ENB_CPP_FEATURES)
    add_subdirectory(${LIB_CPPFEATURES})
    list(APPEND LIB_LIST ${LIB_CPPFEATURES})
endif()

####################################
# Macro 4: ENB_EFFECTIVE_MODERN_CPP
####################################
# Define a macro to enable testing for the reading minutes of EffectiveModernCpp (src/EffectiveModernCpp)
option(ENB_EFFECTIVE_MODERN_CPP "Reading minutes of EffectiveModernCpp" ON)
# add the EffectiveModernCpp library
set(LIB_EFFECTIVE_MODERN_CPP "EffectiveModernCpp")
if(ENB_EFFECTIVE_MODERN_CPP)
    add_subdirectory(${LIB_EFFECTIVE_MODERN_CPP})
    list(APPEND LIB_LIST ${LIB_EFFECTIVE_MODERN_CPP})
endif()

####################################
# Macro 5: ENB_PYRUN_CPP
####################################
# Define a macro to enable testing for the pyrun related code (src/pyrun)
option(ENB_PYRUN_CPP "Python code related" ON)
# add the pyrun library
set(LIB_PYRUN_CPP "pyrun")
if(ENB_PYRUN_CPP)
    add_subdirectory(${LIB_PYRUN_CPP})
    list(APPEND LIB_LIST ${LIB_PYRUN_CPP})
endif()

# DEBUG: Show the value of the LIB_LIST
MESSAGE(STATUS "[PYRAD] LIB_LIST = ${LIB_LIST}")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(projectconfig.h.in projectconfig.h)

AUX_SOURCE_DIRECTORY(./ MY_DIR_SRC)

# Set the binary/executable name 
set(MY_EXEC_NAME "mymainrun")

# Add the executable
ADD_EXECUTABLE(${MY_EXEC_NAME} ${MY_DIR_SRC})

# Set link library
target_link_libraries(${MY_EXEC_NAME} PUBLIC ${LIB_LIST})

# # Create a directory list to append path for linking
set(LINK_DIRS "")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/normal/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/boosttest/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/EffectiveModernCpp/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/cppfeatures/")
list(APPEND LINK_DIRS "${PROJECT_BINARY_DIR}/pyrun/")
MESSAGE(STATUS "[PYRAD] LINK_DIRS = ${LINK_DIRS}")
#target_link_directorie${PROJECT_BINARY_DIR}s(${MY_EXEC_NAME} PUBLIC ${MYCPP11TEST_BINARY_DIR}/EffectiveModernCpp )
target_link_directories(${MY_EXEC_NAME} PUBLIC ${LINK_DIRS} )

# The name '${MY_EXEC_NAME}' MUST have been declared by add_executable() or add_library()
#target_include_directories(${MY_EXEC_NAME} PUBLIC "${PROJECT_BINARY_DIR}")
set(INC_DIRS "")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}/normal/")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}/boosttest/")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}/EffectiveModernCpp/")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}/cppfeatures/")
list(APPEND INC_DIRS "${PROJECT_BINARY_DIR}/pyrun/")
MESSAGE(STATUS "[PYRAD] INC_DIRS = ${INC_DIRS}")
target_include_directories(${MY_EXEC_NAME} PUBLIC ${INC_DIRS})


MESSAGE(STATUS "[PYRAD]  ====== Compiler Information ======")
MESSAGE(STATUS "[PYRAD]  CMAKE_MAKE_PROGRAM : ${CMAKE_MAKE_PROGRAM}")
MESSAGE(STATUS "[PYRAD]  CMAKE_C_COMPILER : ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "[PYRAD]  CMAKE_CXX_COMPILER : ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "[PYRAD]  CMAKE_C_COMPILER_VERSION : ${CMAKE_C_COMPILER_VERSION}")
MESSAGE(STATUS "[PYRAD]  CMAKE_CXX_COMPILER_VERSION : ${CMAKE_CXX_COMPILER_VERSION}")

MESSAGE(STATUS "[PYRAD] ============= ============= ============ =============") 
MESSAGE(STATUS "[PYRAD] ============= [Main CMakeLists.txt ends] =============") 
MESSAGE(STATUS "[PYRAD] ============= ============= ============ =============") 


