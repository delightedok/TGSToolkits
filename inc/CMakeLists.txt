include(./iters/cmake/source.cmake)

if(EXPORT_SORTS)
    include(./sorts/cmake/source.cmake)
endif()

set(TGSTK_HEADER_FILES_H
    tgstk_comms.h
    ${TGSTK_COMMS_HEADER_FILES_H}
    ${TGSTK_ITERS_HEADER_FILES_H}
    ${TGSTK_SORTS_HEADER_FILES_H}
)

install(FILES ${TGSTK_HEADER_FILES_H}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)