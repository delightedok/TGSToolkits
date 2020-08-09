use_c99()

include(./cmake/source.cmake)

set(TESTER_ITERS_LIBS
    ${TESTER_LIBS}
)

set(TESTER_ITERS_INCLUDE_DIRS
)

utils_apply_compile_flags(TESTER_ITERS_SOURCE_FILES_CPP STRICT_OPTIONS_CPP STRICT_OPTIONS_C)

foreach(filename ${TESTER_ITERS_SOURCE_FILES_CPP})
    set(exe_name )
    get_file_basename(exe_name ${filename})
    set(TESTER_ITERS_SOURCES_FILES
        ${TESTER_COMMS_SOURCE_FILES_CPP}
        ${filename}
    )
    add_executable(${exe_name} ${TESTER_ITERS_SOURCES_FILES})
    target_include_directories(${exe_name} PUBLIC ${TESTER_ITERS_INCLUDE_DIRS})
    target_link_libraries(${exe_name} PRIVATE ${TESTER_ITERS_LIBS})
    install(TARGETS ${exe_name}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    )
endforeach(filename)
