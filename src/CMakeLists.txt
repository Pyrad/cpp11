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

#set(BOOST_ROOT /home/pyrad/procs/boost_1_73_0)
#set(BOOST_INCLUDEDIR /home/pyrad/procs/boost_1_73_0/include)
#set(BOOST_LIBRARYDIR /home/pyrad/procs/boost_1_73_0/lib)

# set(BOOST_ROOT /home/pyrad/procs/boost_1_69_0)
# set(BOOST_INCLUDEDIR /home/pyrad/procs/boost_1_69_0/include)
# set(BOOST_LIBRARYDIR /home/pyrad/procs/boost_1_69_0/lib)

set(BOOST_ROOT /home/pyrad/procs/boost_1_69_0_v1)
#set(BOOST_INCLUDEDIR /home/pyrad/procs/boost_1_69_0/include)
#set(BOOST_LIBRARYDIR /home/pyrad/procs/boost_1_69_0/lib)

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


AUX_SOURCE_DIRECTORY(./ MY_DIR_SRC)

# Add the executable
#ADD_EXECUTABLE(mymainrun main.cpp)
ADD_EXECUTABLE(mymainrun ${MY_DIR_SRC})


#target_link_directories(mymainrun PUBLIC ${MYCPP11TEST_BINARY_DIR})

# The name 'mymainrun' MUST have been declared by add_executable() or add_library()
#target_include_directories(mymainrun PUBLIC "${PROJECT_BINARY_DIR}" )



