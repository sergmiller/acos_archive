#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "string_for_text_editor.h"
#include "cartesian_tree.h"
#include "insert_functions.h"
#include "replace_function.h"
#include "my_print_pages.h"
#include "editor.h"
#include "filework.h"

/*get size_t number from string*/
int get_index(size_t* num_val, char* num_ptr)
{
    size_t cnt = 0;
    short dig;
    size_t resalt = 0;

    if(num_ptr == NULL)
        return -1;

    if(*num_ptr == '\0')
        return 2;

    while( *num_ptr != ' ' && *num_ptr != '\0')
    {
        if(*num_ptr == '#')
        {
            *num_val = resalt;
            return 3;
        }

        resalt *= 10;
        dig = (short)(*num_ptr - '0');
        
        if(cnt++ > 18 || dig < 0 || dig > 9)
            return 0;
        
        resalt += dig;
        ++num_ptr;
    }
    *num_val = resalt;
    return 1;
}

/*all work to parse two index in 3 and 4 position*/
short total_parse_2index(size_t* num1_val_ptr, size_t* num2_val_ptr, char* str, cartesian_tree_t* data_tree)
{
    char* num1_ptr = NULL;
    char* num2_ptr = NULL;
    size_t num1_val = 0;
    size_t num2_val = 0;
    int flg = 0;
    /** size_t i = 0;**/

    num1_ptr = get_word_in_next_pos(str);
    num2_ptr = get_word_in_next_pos(num1_ptr);

    flg = get_index(&num1_val, num1_ptr);

    if(flg == -1)
    {
        fprintf(stderr, "Can't do this operation\n");
        return 0;
    }

    if(!flg) 
    {
        fprintf(stderr, "It isn't number of page or position.\nPlease input correct number\n");
        return 0;
    }

    if(*num1_ptr == '\0' || *num1_ptr == '#')
        num1_val = 1;
    /*
    if(flg == 2)
    {
        fprintf(stderr, "Don't input number of page or position\nPlease input correct number\n");
        return 2;
    }
    */

    flg = get_index(&num2_val, num2_ptr);
       
    if(!flg) 
    {   
        fprintf(stderr, "It isn't number of page or position.\nPlease input correct number\n");
        return 0;
    }

    if(*num2_ptr == '\0' || *num2_ptr == '#')
        num2_val = data_tree->size;

    if(!num1_val || num1_val > num2_val || num1_val == 0 || num2_val > data_tree->size)
    {
        fprintf(stderr, "It isn't number of page or position.\nPlease input correct number\n");
        return 0;
    }

    *num1_val_ptr = num1_val;
    *num2_val_ptr = num2_val;

    return 1;
}

int correct_next_symbol(char* smb)
{   
    if(smb == NULL)
        return -1;

    if(*smb == '\0' || *smb == ' ' || *smb == '#')
        return 0;

    return 1;
}


/**subscribe of general parse function**/
short parse(string_t* str, cartesian_tree_t* help_file, cartesian_tree_t** data_tree_ptr, char** last_file_ptr)
{
    char* first_word = str->data;
    char* second_word = NULL;
    cartesian_tree_t* data_tree = *data_tree_ptr;

    if(*(str->data) == ' ')
        first_word = get_word_in_next_pos(str->data);
    
    second_word = get_word_in_next_pos(first_word);

    if(!strncmp(first_word, "exit", 4) && !correct_next_symbol(first_word + 4))
    {
        return 0;
    }

    if(!strncmp(first_word, "read", 4) && !correct_next_symbol(first_word + 4))
    {
        return my_read_and_open(second_word, data_tree_ptr, NULL, READ);
    }

    if(!strncmp(first_word, "open", 4) && !correct_next_symbol(first_word + 4))
    {
        return my_read_and_open(second_word, data_tree_ptr, last_file_ptr, OPEN);
    }

    if(!strncmp(first_word, "write", 5) && !correct_next_symbol(first_word + 5))
    {
        return my_write(second_word, data_tree_ptr, last_file_ptr);
    }

    if(!strncmp(first_word, "help", 4) && !correct_next_symbol(first_word + 4))
    {
        print_pages(help_file, 0);
        return 1;
    }


    if(!strncmp(first_word, "replace", 7) && !correct_next_symbol(first_word + 7))
    {
        return replace_func(str, data_tree_ptr, first_word, second_word);
    }

    if(!strncmp(first_word, "delete", 6) && !strncmp(second_word, "range", 5) && !correct_next_symbol(second_word + 5))
    {
        if(!data_tree)
        {
            printf("Can't doing something with range from empty set of strings\n");
            return 1;
        }
        else
        {
            /**get number of first deleted string**/
            size_t num1_val = 0;
            size_t num2_val = 0;

            char cmd = 0;

            if(total_parse_2index(&num1_val, &num2_val, second_word, *data_tree_ptr) == 1)
            {
                if(num1_val == 1 && num2_val == data_tree->size)
                {
                    fprintf(stdout, "Delete all lines. Are you sure?(y/n):\n");
                    while(1)
                    {
                        cmd = getchar();
                        if(cmd == EOF)
                        {
                            fprintf(stdout,"stdin closed at commands parsing\n");
                            return 1;
                        }

                        if(cmd == 'n' || cmd == 'N')
                        {
                            cmd = getchar();
                            return 1;
                        }
                        if(cmd == 'y' || cmd == 'Y')
                        {
                            cmd = getchar();
                            break;
                        }
                    }
                }
                cart_tree_universal_action_with_subtree(data_tree_ptr, num1_val, num2_val, DELETE_SUBTREE, NULL, NULL);
            }
            return 1;
        }
    }
    
    if(!strncmp(first_word, "delete", 6) && !strncmp(second_word, "braces", 6) && !correct_next_symbol(second_word + 6))
    {
        if(!data_tree)
        {
            printf("Can't doing something with range from empty set of strings\n");
            return 1;
        }
        else
        {
            /**get number of first deleted string**/
            size_t num1_val = 0;
            size_t num2_val = 0;

            int flg = total_parse_2index(&num1_val, &num2_val, second_word, *data_tree_ptr);
            if(flg == 2)
            {
                num1_val = 1;
                num2_val = data_tree->size;
            }
                if(flg)
                    cart_tree_universal_action_with_subtree(data_tree_ptr, num1_val, num2_val, DELETE_BRACES, NULL, NULL);

            return 1;
        }
    }

    if(!strncmp(first_word, "print", 5) && !strncmp(second_word, "range", 5)  && !correct_next_symbol(second_word + 5))
    {
        if(!data_tree)
        {
            printf("Can't doing something with range from empty set of strings\n");
            return 1;
        }
        else
        {
            /**get number of first deleted string**/
            size_t num1_val = 0;
            size_t num2_val = 0;

            if(total_parse_2index(&num1_val, &num2_val, second_word, *data_tree_ptr) == 1)
                cart_tree_universal_action_with_subtree(data_tree_ptr, num1_val, num2_val, PRINT, NULL, NULL);
            return 1;
        }
    }


    if(!strncmp(first_word, "edit", 4) && !strncmp(second_word, "string", 6)  && !correct_next_symbol(second_word + 6))
    {
        if(data_tree)
            edit_string(str, data_tree_ptr, second_word);
        else
            printf("Can't edit empty set of strings\n");
        return 1;
    }

    if(!strncmp(first_word, "print", 5) && !strncmp(second_word, "pages", 5)  && !correct_next_symbol(second_word + 5))
    {
        print_pages(data_tree, 1);
        
        return 1;
    }

    if(!strncmp(first_word, "insert", 6) && !strncmp(second_word, "after", 5)  && !correct_next_symbol(second_word + 5))
    {
        return insert_after (str, data_tree_ptr, second_word);
    }
    
    if(*first_word != '\0' && *first_word != '#')
        fprintf(stderr, "\"%s\" - command not found\n", str->data);
    return 1;
}

