use_c99()

include(./comms/cmake/source.cmake)

if(EXPORT_SORTS)
    include(./sorts/cmake/source.cmake)
endif()

set(TESTER_SOURCE_FILES_CPP
    ${TESTER_COMMS_SOURCE_FILES_CPP}
    ${TESTER_SORTS_SOURCE_FILES_CPP}
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

add_executable(tester_sorts ${TESTER_SOURCE_FILES_CPP})
target_include_directories(tester_sorts PUBLIC ${TESTER_INCLUDE_DIRS})
target_link_libraries(tester_sorts PRIVATE ${TESTER_LIBS})
install(TARGETS tester_sorts
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)