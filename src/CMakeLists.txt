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

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows" AND ${MINGW} EQUAL 1)
	MESSAGE(STATUS "[PYRAD] Windows MinGW system!")
	MESSAGE(STATUS "[PYRAD] Assume current is MSYS2")
	# NOTE, the boost should be compiled with the following command
	# b2.exe install --layout=system threading=multi variant=release \
	#                link=shared toolset=gcc address-model=64 \
	#                --build-dir="D:\procs\boost_1_79_0_src\PyradBuild" \
	#                --prefix="D:\procs\boost_1_79_0_system"
	#set(MY_BOOST_HOME_DIR "D:/procs/boost_1_79_0_system")
	set(MY_BOOST_HOME_DIR "D:/procs/boost_1_79_0")
	set(BOOST_ROOT ${MY_BOOST_HOME_DIR})
	set(BOOST_INCLUDEDIR ${MY_BOOST_HOME_DIR}/include)
	set(BOOST_LIBRARYDIR ${MY_BOOST_HOME_DIR}/lib)
	set(Boost_USE_STATIC_LIBS OFF)
elseif(${UNIX} STREQUAL "Unix")
	MESSAGE(STATUS "[PYRAD] Unix system!")
	set(BOOST_ROOT /home/pyrad/procs/boost_1_69_0)
endif()

#set(BOOST_ROOT /home/pyrad/procs/boost_1_73_0)
#set(BOOST_INCLUDEDIR /home/pyrad/procs/boost_1_73_0/include)
#set(BOOST_LIBRARYDIR /home/pyrad/procs/boost_1_73_0/lib)

# set(BOOST_ROOT /home/pyrad/procs/boost_1_69_0)
# set(BOOST_INCLUDEDIR /home/pyrad/procs/boost_1_69_0/include)
# set(BOOST_LIBRARYDIR /home/pyrad/procs/boost_1_69_0/lib)

#set(BOOST_ROOT /home/pyrad/procs/boost_1_69_0_v1)
#set(BOOST_ROOT /home/pyrad/procs/boost_1_69_0)
#set(BOOST_INCLUDEDIR /home/pyrad/procs/boost_1_69_0/include)
#set(BOOST_LIBRARYDIR /home/pyrad/procs/boost_1_69_0/lib)

# set(MY_BOOST_HOME_DIR "D:/procs/boost_1_79_0_system")
# #set(MY_BOOST_HOME_DIR " /d/procs/boost_1_79_0")
# set(BOOST_ROOT ${MY_BOOST_HOME_DIR})
# set(BOOST_INCLUDEDIR ${MY_BOOST_HOME_DIR}/include)
# set(BOOST_LIBRARYDIR ${MY_BOOST_HOME_DIR}/lib)
# #set(Boost_LIB_PREFIX "lib")
# #set(Boost_USE_STATIC_LIBS ON)
# #set(Boost_USE_STATIC_RUNTIME ON)

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



