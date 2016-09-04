#ifndef __EDITOR__
#define __EDITOR__

#include <stdio.h>
#include <stdlib.h>
#include "string_for_text_editor.h"
#include "cartesian_tree.h"

void get_pref_function(string_t* str, int** resalt);
void replace_substr(string_t* inserted_str, string_t* pref_str_ptr, string_t** scan_str_ptr, int** pref_func);
char* get_next_substr(char* scan_str, char* pref_str, size_t finding_str_size, int** pref_func);
#endif
