set(_TGSTK_COMMS_HEADER_FILES_H
    bubble_sort.h
    cocktail_shaker_sort.h
    gnome_sort.h
)

set(TGSTK_COMMS_HEADER_FILES_H )
add_head_foreach(TGSTK_COMMS_HEADER_FILES_H "sorts" ${_TGSTK_COMMS_HEADER_FILES_H})