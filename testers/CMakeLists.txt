use_c99()

include(./comms/cmake/source.cmake)

set(TESTER_SOURCE_FILES_CPP
    ${TESTER_COMMS_SOURCE_FILES_CPP}
)

set(TESTER_LIBS
)

set(TESTER_INCLUDE_DIRS
)

utils_apply_compile_flags(TESTER_SOURCE_FILES_CPP STRICT_OPTIONS_CPP STRICT_OPTIONS_C)

if(ENABLE_STATIC)
    list(APPEND TESTER_LIBS toolkit-static)
elseif(ENABLE_SHARED)
    list(APPEND TESTER_LIBS toolkit)
endif()

if(EXPORT_SORTS)
    add_subdirectory(sorts)
endif()
