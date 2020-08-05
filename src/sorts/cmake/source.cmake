set(_TGSTK_SORTS_SOURCE_FILES_CPP
    sort_comms.cpp
    BubbleSort/bubble_sort.cpp
    CocktailShakerSort/cocktail_shaker_sort.cpp
    GnomeSort/gnome_sort.cpp
)

set(TGSTK_SORTS_SOURCE_FILES_CPP )
add_head_foreach(TGSTK_SORTS_SOURCE_FILES_CPP "sorts" ${_TGSTK_SORTS_SOURCE_FILES_CPP})