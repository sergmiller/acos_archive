#define __PRINT_PAGES__
#ifndef __PRINT_PAGES__

#include <stdio.h>
#include <stdlib.h>

unsigned short my_log(size_t n);
void print_pages(cartesian_tree_t* tree, size_t start);
short window_string_print(cartesian_tree_t** ptr, size_t* range_ptr, size_t* ind, unsigned short* cur_row_ptr, unsigned short cur_col)
#endif