include(./iters/list/cmake/source.cmake)

set(_TGSTK_ITERS_HEADER_FILES_H
    ${TGSTK_ITERS_LIST_HEADER_FILES_H}
)

set(TGSTK_ITERS_HEADER_FILES_H )
add_head_foreach(TGSTK_ITERS_HEADER_FILES_H "iters" ${_TGSTK_ITERS_HEADER_FILES_H})