################################################################
###
### The purpose of this CMake script is to test CMake features
### Use "cmake -P <cmake_script>" to run
###
################################################################

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