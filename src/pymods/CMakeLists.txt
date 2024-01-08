# Set the name of current Python library
set(CurPyLibName "pymod")

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR ${CurPyLibName}] -------------") 
MESSAGE(STATUS "[PYRAD]   CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

# Copy Python run script from source dir to build dir (2024-01-06)
# Note here CMAKE_SOURCE_DIR is ${GIT_REPO_ROOT}/src, and CMAKE_CURRENT_BINARY_DIR
# is ${GIT_REPO_ROOT}/build/pyrun
add_custom_command(TARGET ${CurSharedLibName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/${CurSharedLibName}/top.py
    ${CMAKE_CURRENT_BINARY_DIR}/top.py)

MESSAGE(STATUS "[PYRAD]   ------------- [SUB DIR ${CurPyLibName}] [END] -------------") 
