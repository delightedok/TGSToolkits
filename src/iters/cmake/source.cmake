include(./iters/list/cmake/source.cmake)

set(_TGSTK_ITERS_SOURCE_FILES_CPP
    ${TGSTK_ITERS_LIST_SOURCE_FILES_CPP}
)

set(TGSTK_ITERS_SOURCE_FILES_CPP )
add_head_foreach(TGSTK_ITERS_SOURCE_FILES_CPP "iters" ${_TGSTK_ITERS_SOURCE_FILES_CPP})