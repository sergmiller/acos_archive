#include "string_for_text_editor.h"
#include "cartesian_tree.h"
#ifndef __PARSER__
#define __PARSER__

enum file_type
{
	TXT,
	BIN
};
typedef enum file_type file_t;

int parse(string_t* s, cartesian_tree_t** data_tree_ptr, char** last_file_ptr, const char* cur_inp, const char* cur_create);
void get_inex(size_t* num_val, char* num_ptr);
int total_parse_2index(size_t* num1_val_ptr, size_t* num2_val_ptr, char* str, cartesian_tree_t* data_tree);
int correct_next_symbol(char* smb);

#endif
