use_c99()

add_compile_options("-funwind-tables")

include(./comms/cmake/source.cmake)

if(EXPORT_SORTS)
    include(./sorts/cmake/source.cmake)
endif()

set(TGSTK_SOURCE_FILES_CPP
    ${TGSTK_COMMS_SOURCE_FILES_CPP}
    ${TGSTK_SORTS_SOURCE_FILES_CPP}
)

set(TGSTK_LIBS
)

set(TGSTK_INCLUDE_DIRS
)

utils_apply_compile_flags(TGSTK_SOURCE_FILES_CPP STRICT_OPTIONS_CPP STRICT_OPTIONS_C)

if(ENABLE_STATIC)
    add_library(toolkit-static STATIC ${TGSTK_SOURCE_FILES_CPP})
    target_link_libraries(toolkit-static INTERFACE ${TGSTK_LIBS})
    target_include_directories(toolkit-static PUBLIC ${TGSTK_INCLUDE_DIRS})
    install(TARGETS toolkit-static EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    )
endif()

if(ENABLE_SHARED)
    add_compile_options("-fvisibility=hidden")
    add_library(toolkit SHARED ${TGSTK_SOURCE_FILES_CPP})
    target_link_libraries(toolkit PRIVATE ${TGSTK_LIBS})
    target_include_directories(toolkit PUBLIC ${TGSTK_INCLUDE_DIRS})
    install(TARGETS toolkit EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    )
endif()