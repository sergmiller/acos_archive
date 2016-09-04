#include <stdio.h>
#include <stdlib.h>
#include "string_for_text_editor.h"

#ifndef __PARSER__
#define __PARSER__

enum file_type
{
	TXT,
	BIN
};

typedef enum file_type file_t;

short parse(string_t* s);
int get_index(size_t* num_val, char* num_ptr);
int correct_next_symbol(char* smb);

#endif
