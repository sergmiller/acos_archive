#include "string_for_text_editor.h"
#include "cartesian_tree.h"
#include <string.h>
#define __FILEWORK__
#ifndef __FILEWORK__
short my_read_and_open(char* fp_ptr, cartesian_tree_t** data_tree_ptr, char** last_str_ptr, const char* cur_inp, action_t action);
short my_write(char* fp_ptr, cartesian_tree_t** data_tree_ptr, char** last_str_ptr, const char* cur_write);
#endif