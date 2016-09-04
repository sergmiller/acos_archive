#define __FILEWORK__
#ifndef __FILEWORK__

#include "cartesian_tree.h"

short my_read_and_open(char* fp_ptr, cartesian_tree_t** data_tree_ptr, char** last_str_ptr, action_t action);
short my_write(char* fp_ptr, cartesian_tree_t** data_tree_ptr, char** last_str_ptr);
#endif