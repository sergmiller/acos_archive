/**simple version of cartesian tree for fast access to saved strings*/
#ifndef __CARTESIAN_TREE__
#define __CARTESIAN_TREE__

#include <stdio.h>
#include <stdlib.h>
#include "string_for_text_editor.h"

/**struct is realized treap on implicit key**/
struct cartesian_tree
{
    int priority;
    size_t size;
    struct cartesian_tree* left;
    struct cartesian_tree* right;
    string_t* text;
};

enum action
{
	DELETE_BRACES,
	DELETE_SUBTREE,
	DELETE,
	REPLACE,
	PRINT,
	PASTE,
	EDIT,
	OPEN,
	READ
};

typedef struct cartesian_tree cartesian_tree_t;
typedef enum action action_t;

struct data_warehouse
{
	cartesian_tree_t* list_begin;
	cartesian_tree_t* list_end;
	string_t* str_ptr;
};

typedef struct data_warehouse data_warehouse_t;


void cart_tree_construct(cartesian_tree_t** tree, string_t* str);
void cart_tree_merge(cartesian_tree_t** merged_tree, cartesian_tree_t** tree1,cartesian_tree_t** tree2);
void cart_tree_split(cartesian_tree_t** splited_tree, cartesian_tree_t** left_tree, cartesian_tree_t** right_tree,size_t index);
void cart_tree_insert(cartesian_tree_t** tree, string_t* str, size_t index);
void cart_tree_insert_tree(cartesian_tree_t** tree, cartesian_tree_t** inserted_tree, size_t index);
void recalc_params(cartesian_tree_t* tree);
void print_tree(cartesian_tree_t* tree, FILE* fp);
void tree_print(cartesian_tree_t* tree);
void tree_delete(cartesian_tree_t* tree);
void list_delete(cartesian_tree_t* list);
void in_order_delete_braces(cartesian_tree_t** tree);
void in_order_replace(cartesian_tree_t** mid_tree, data_warehouse_t* replaced_str_data, data_warehouse_t* scan_str_data);
void conv_tree_to_list(cartesian_tree_t** list_ptr, cartesian_tree_t** end_list_ptr, cartesian_tree_t** tree);
void conv_list_to_tree(cartesian_tree_t** list_ptr, cartesian_tree_t** end_list_ptr, cartesian_tree_t** tree);
void cart_tree_access(cartesian_tree_t** tree_ptr, size_t index, string_t** s);
/**void cart_tree_assign(cartesian_tree_t* tree, char* str, size_t sz, size_t index);**/
void dfs_replace(cartesian_tree_t** tree_ptr, string_t* replaced_str, string_t* scan_str, int** pref);
void cart_tree_universal_action_with_subtree(cartesian_tree_t** tree, size_t left_ind, size_t right_ind, action_t action,  data_warehouse_t* buff1, data_warehouse_t* buff2);
void insert_copy_list(cartesian_tree_t* first_cur_ptr, cartesian_tree_t* insert_list_begin, cartesian_tree_t* insert_list_end);

short my_read_and_open(char* fp_ptr, cartesian_tree_t** data_tree_ptr, char** last_str_ptr, action_t action);
short my_write(char* fp_ptr, cartesian_tree_t** data_tree_ptr, char** last_str_ptr);
void print_pages(cartesian_tree_t* tree, size_t start);
#endif
