#include "string_for_text_editor.h"
#include "cartesian_tree.h"
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#define __PRINT_PAGES__
#ifndef __PRINT_PAGES__
void print_pages(cartesian_tree_t* tree, size_t start);
short window_string_print(cartesian_tree_t** ptr, size_t* range_ptr, size_t* ind, short* log_size, unsigned short* cur_row_ptr, unsigned short cur_col)
#endif