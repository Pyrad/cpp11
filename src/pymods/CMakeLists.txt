# Formerly a broken dead configuration: it referenced ${CurSharedLibName},
# which is defined in pyrun/CMakeLists.txt and is not visible here, and the
# directory was never added with add_subdirectory(). It is now a plain,
# self-contained copy step with no target dependency.
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/top.py
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/top.py
        COPYONLY)
