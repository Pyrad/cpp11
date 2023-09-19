################################################################
###
### The purpose of this CMake script is to test CMake features
### Use "cmake -P <cmake_script>" to run
###
################################################################
SET(CMAKE_CXX_STANDARD 14)

MESSAGE(STATUS "[PYRAD] CYGWIN: " ${CYGWIN})
MESSAGE(STATUS "[PYRAD] MINGW: " ${MINGW})
MESSAGE(STATUS "[PYRAD] MSYS: " ${MSYS})
MESSAGE(STATUS "[PYRAD] UNIX: " ${UNIX})
MESSAGE(STATUS "[PYRAD] CMAKE_SYSTEM: " ${CMAKE_SYSTEM})
MESSAGE(STATUS "[PYRAD] CMAKE_SYSTEM_NAME: " ${CMAKE_SYSTEM_NAME})
MESSAGE(STATUS "[PYRAD] CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR})
MESSAGE(STATUS "[PYRAD] CMAKE_HOST_SYSTEM: " ${CMAKE_HOST_SYSTEM})
MESSAGE(STATUS "[PYRAD] CMAKE_HOST_SYSTEM_NAME: " ${CMAKE_HOST_SYSTEM_NAME})

cmake_host_system_information(RESULT PRETTY_NAME QUERY DISTRIB_PRETTY_NAME)
message(STATUS "[PYRAD] PRETTY_NAME:" ${PRETTY_NAME})
cmake_host_system_information(RESULT DISTRO QUERY DISTRIB_INFO)
foreach(VAR IN LISTS DISTRO)
  message(STATUS "[PYRAD]   ${VAR}=`${${VAR}}`")
endforeach()

# A portable way to check if 32-bit or 64-bit platform
# If it is 4, the platform is 32-bit; if it is 8, the platform is 64-bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(bstring "64-bit platform")
else()
    SET(bstring "32-bit platform")
endif()
MESSAGE(STATUS "[PYRAD] CMAKE_SIZEOF_VOID_P = ${CMAKE_SIZEOF_VOID_P} (${bstring})")

### Print out some host information
### Looks nothing got on Windows?
MESSAGE(STATUS "[PYRAD]")
MESSAGE(STATUS "[PYRAD] ===== Some info got by cmake_host_system_information =====")
cmake_host_system_information(RESULT PRETTY_NAME QUERY DISTRIB_PRETTY_NAME)
MESSAGE(STATUS "[PYRAD] PRETTY_NAME: ${PRETTY_NAME}")
cmake_host_system_information(RESULT DISTRO QUERY DISTRIB_INFO)
foreach(VAR IN LISTS DISTRO)
    MESSAGE(STATUS "[PYRAD]   [DISTRO] ${VAR} = `${${VAR}}`")
endforeach()

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
LIST(LENGTH QueryKeyList klen)
MESSAGE(STATUS "[PYRAD] Query key list length: ${klen}")
foreach( cur_val ${QueryKeyList})
    CMAKE_HOST_SYSTEM_INFORMATION(RESULT qval QUERY ${cur_val})
    MESSAGE(STATUS "[PYRAD] [QueryKeyList]  ${cur_val} : ${qval}")
endforeach()


# Set Python3_ROOT_DIR if Python is installed in a non-system path
#set(Python3_ROOT_DIR "D:/procs/python38")
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
# add_executable(cpp-python-test main.cpp)
# target_include_directories(${PROJECT_NAME} PRIVATE ${Python3_INCLUDE_DIRS})
# target_link_libraries(${PROJECT_NAME} ${Python3_LIBRARIES})

MESSAGE(STATUS "[PYRAD]                Python3_Found: ${Python3_Found}" )
MESSAGE(STATUS "[PYRAD]    Python3_Development_FOUND: ${Python3_Development_FOUND}")
MESSAGE(STATUS "[PYRAD]         Python3_INCLUDE_DIRS: ${Python3_INCLUDE_DIRS}")
MESSAGE(STATUS "[PYRAD]            Python3_LIBRARIES: ${Python3_LIBRARIES}")
MESSAGE(STATUS "[PYRAD]         Python3_LIBRARY_DIRS: ${Python3_LIBRARY_DIRS}")
MESSAGE(STATUS "[PYRAD] Python3_RUNTIME_LIBRARY_DIRS: ${Python3_RUNTIME_LIBRARY_DIRS}")
MESSAGE(STATUS "[PYRAD]              Python3_VERSION: ${Python3_VERSION}")


# SET(MY_MINGW64_HOME "/mingw64")
# SET(CMAKE_MAKE_PROGRAM "${MY_MINGW64_HOME}/bin/mingw32-make.exe")
# SET(CMAKE_C_COMPILER "${MY_MINGW64_HOME}/bin/gcc.exe")
# SET(CMAKE_CXX_COMPILER "${MY_MINGW64_HOME}/bin/g++.exe")

MESSAGE(STATUS "[PYRAD]  ====== Compiler Information ======")
MESSAGE(STATUS "[PYRAD]  CMAKE_MAKE_PROGRAM : ${CMAKE_MAKE_PROGRAM}")
MESSAGE(STATUS "[PYRAD]  CMAKE_C_COMPILER : ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "[PYRAD]  CMAKE_CXX_COMPILER : ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "[PYRAD]  CMAKE_C_COMPILER_VERSION : ${CMAKE_C_COMPILER_VERSION}")
MESSAGE(STATUS "[PYRAD]  CMAKE_CXX_COMPILER_VERSION : ${CMAKE_CXX_COMPILER_VERSION}")



