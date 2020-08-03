set(_TGSTK_SORTS_SOURCE_FILES_CPP
    BubbleSort/bubble_sort.cpp
    sort_comms.cpp
)

set(TGSTK_SORTS_SOURCE_FILES_CPP )
add_head_foreach(TGSTK_SORTS_SOURCE_FILES_CPP "sorts" ${_TGSTK_SORTS_SOURCE_FILES_CPP})