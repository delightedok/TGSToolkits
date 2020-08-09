set(_TGSTK_ITERS_LIST_SOURCE_FILES_CPP
    list_comms.cpp
    NormalList/normal_list.cpp
)

set(TGSTK_ITERS_LIST_SOURCE_FILES_CPP )
add_head_foreach(TGSTK_ITERS_LIST_SOURCE_FILES_CPP "list" ${_TGSTK_ITERS_LIST_SOURCE_FILES_CPP})