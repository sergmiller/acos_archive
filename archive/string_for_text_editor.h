#include <stdio.h>
#include <stdlib.h>

/**auxiliary library base lines with support for string operations**/
#ifndef __MY_STR__
#define __MY_STR__


#define SIZE_OF_STRING_PARTS 300

/**char array with dynamic size and stirng operations**/
struct string
{
    size_t full_size;
    size_t size;
    char* data;
};

typedef struct string string_t;

void string_construct(string_t** str);
char* string_begin(string_t* str);
char* string_big_push(string_t* str, char* m_push, size_t m_size);
void string_big_remove(string_t* str, char* pos, size_t size);
void string_big_insert(string_t* str, size_t pos, char* insert_str, size_t size);
void string_push_back(string_t* str, char chr);
void string_pop_back(string_t* str);
void string_insert(string_t* str, char chr, size_t index);
void string_assign(string_t* str, char chr, size_t index);
void string_remove(string_t* str, size_t index);
void string_print(string_t* str, FILE* fp);
void string_delete(string_t* str);
int string_scan_braces(int* deep_level, string_t* str);
void string_delete_braces(int* deep_level, string_t* str);
int string_compare(string_t* str, size_t shift, const char* str_cmp);
char* get_word_in_next_pos(char* res_ptr);
int get_next_string(string_t* str, FILE* fp);
void string_replace(string_t* source_str,size_t position, string_t* replace_str, size_t size);
#endif
