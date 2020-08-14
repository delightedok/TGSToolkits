set(_TGSTK_SORTS_HEADER_FILES_H
    bubble_sort.h
    cocktail_shaker_sort.h
    gnome_sort.h
    optimized_gnome_sort.h
    odd_even_sort.h
    comb_sort.h
    quick_sort.h
    stable_quick_sort.h
    dual_pivot_quick_sort.h
)

set(TGSTK_SORTS_HEADER_FILES_H )
add_head_foreach(TGSTK_SORTS_HEADER_FILES_H "sorts" ${_TGSTK_SORTS_HEADER_FILES_H})