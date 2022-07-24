#############################################################################################
### How to make this project ?
###
###  $ cmake -G Ninja <path-to-this-cmake-file> -DCMAKE_BUILD_TYPE=Release -DBoost_DEBUG=ON
###  $ cmake --build .
###
#############################################################################################

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

MESSAGE(STATUS "[PYRAD] This is the BINARY directory: " ${MYCPP11TEST_BINARY_DIR})
MESSAGE(STATUS "[PYRAD] This is the SOURCE directory: " ${MYCPP11TEST_SOURCE_DIR})
MESSAGE(STATUS "[PYRAD] CYGWIN: " ${CYGWIN})
MESSAGE(STATUS "[PYRAD] MINGW: " ${MINGW})
MESSAGE(STATUS "[PYRAD] MSYS: " ${MSYS})
MESSAGE(STATUS "[PYRAD] UNIX: " ${UNIX})
MESSAGE(STATUS "[PYRAD] CMAKE_SYSTEM: " ${CMAKE_SYSTEM})
MESSAGE(STATUS "[PYRAD] CMAKE_SYSTEM_NAME: " ${CMAKE_SYSTEM_NAME})
MESSAGE(STATUS "[PYRAD] CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR})
MESSAGE(STATUS "[PYRAD] CMAKE_HOST_SYSTEM: " ${CMAKE_HOST_SYSTEM})
MESSAGE(STATUS "[PYRAD] CMAKE_HOST_SYSTEM_NAME: " ${CMAKE_HOST_SYSTEM_NAME})

### Print out some host information
### Looks nothing got on Windows?
cmake_host_system_information(RESULT PRETTY_NAME QUERY DISTRIB_PRETTY_NAME)
message(STATUS "[PYRAD] PRETTY_NAME:" ${PRETTY_NAME})
cmake_host_system_information(RESULT DISTRO QUERY DISTRIB_INFO)
foreach(VAR IN LISTS DISTRO)
  message(STATUS "[PYRAD]   ${VAR}=`${${VAR}}`")
endforeach()



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


if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" AND ${MINGW} EQUAL 1)
	MESSAGE(STATUS "[PYRAD] Windows MinGW system!")
	MESSAGE(STATUS "[PYRAD] Assume current is MSYS2")
	set(MY_BOOST_HOME_DIR "D:/procs/boost_1_79_0")
	set(BOOST_ROOT ${MY_BOOST_HOME_DIR})
	set(BOOST_INCLUDEDIR ${MY_BOOST_HOME_DIR}/include)
	set(BOOST_LIBRARYDIR ${MY_BOOST_HOME_DIR}/lib)
	set(Boost_USE_STATIC_LIBS OFF)
elseif(${UNIX} STREQUAL "Unix")
	MESSAGE(STATUS "[PYRAD] Unix system!")
	set(BOOST_ROOT /home/pyrad/procs/boost_1_69_0)
endif()

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

# Define a macro to enable testing for CPP feature testing (src/cppfeatures)
option(ENB_CPP_FEATURES "Testing cpp features" ON)
# add the MathFunctions library
if(ENB_CPP_FEATURES)
  add_subdirectory(cppfeatures)
  list(APPEND CPPFEATURES_LIB cppfeatures)
endif()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(projectconfig.h.in projectconfig.h)

AUX_SOURCE_DIRECTORY(./ MY_DIR_SRC)

# Set the project name 
set(MY_CUR_PROJ_NAME "mymainrun")

# Add the executable
ADD_EXECUTABLE(${MY_CUR_PROJ_NAME} ${MY_DIR_SRC})

# Set link library
target_link_libraries(${MY_CUR_PROJ_NAME} PUBLIC ${CPPFEATURES_LIB})

#target_link_directories(${MY_CUR_PROJ_NAME} PUBLIC ${MYCPP11TEST_BINARY_DIR})

# The name '${MY_CUR_PROJ_NAME}' MUST have been declared by add_executable() or add_library()
target_include_directories(${MY_CUR_PROJ_NAME} PUBLIC "${PROJECT_BINARY_DIR}" )



