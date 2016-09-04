#include "parser.h"
#include "editor.h"
#include "insert_functions.h"


short insert_after (string_t* str, cartesian_tree_t** data_tree_ptr, char* second_word)
{
    string_t* real_insert_str = NULL;

    cartesian_tree_t* data_tree = *data_tree_ptr;
    cartesian_tree_t* real_insert_tree = NULL;

    size_t index = 0;
    size_t real_tree_size = (data_tree == NULL ? 0 : data_tree->size);
    size_t qut_str = 0;
    short cnt_str_group = 0;

    char* start_ins_ptr = get_word_in_next_pos(second_word);
    char* insert_ptr = 0;

    if(start_ins_ptr == '\0')
    {
        fprintf(stderr,"Please input string\n");
        return 1;
    }

    if(*start_ins_ptr == '\"')
        index = real_tree_size;
    else
    {
        if(get_index(&index, get_word_in_next_pos(second_word)))
            start_ins_ptr = get_word_in_next_pos(start_ins_ptr);

        if(index < 0 || index > real_tree_size)
        {
            fprintf(stderr,"Please input correct range\n");
            return 1;
        }
    }

    if(*start_ins_ptr != '\"')
    {
        fprintf(stderr, "Inserted string looks like \"...\" or \"\"\"...\"\"\" braces\nPlease input correct string\n");
        return 1;
    }

    if(*(start_ins_ptr + 1) == '\"' && *(start_ins_ptr + 2) == '\"')
    {
        cnt_str_group = 1;
        start_ins_ptr += 2;
    }

    ++start_ins_ptr;
    insert_ptr = start_ins_ptr;

    string_construct(&real_insert_str);

    while(1)
    {
        if(*insert_ptr == '\0')
        {
            if(!cnt_str_group)
            {
                fprintf(stderr, "Inserted string looks like \"...\" or ""\"...""\" braces\nPlease input correct string\n");
                break;
            }
            else
            {
                cart_tree_insert(&real_insert_tree, real_insert_str, qut_str);
                string_construct(&real_insert_str);
                string_construct(&str);

                if(get_next_string(str, stdin))
                {
                    return 0;
                }
                start_ins_ptr = str->data;
                insert_ptr = start_ins_ptr;
                ++qut_str;
            }
        }

        if(*insert_ptr == '\"')
        {
            if(!cnt_str_group)
            {
                cart_tree_insert(data_tree_ptr, real_insert_str, index);
                break;
            }
            else
            {
                if(*(insert_ptr + 1) == '\"' && *(insert_ptr + 2) == '\"')
                {
                    cart_tree_insert(&real_insert_tree, real_insert_str, qut_str);
                    cart_tree_insert_tree(data_tree_ptr, &real_insert_tree, index);
                    break;
                }

                string_push_back(real_insert_str, *insert_ptr);
                ++insert_ptr;
                continue;
            }
        }


        if(*insert_ptr == '\\' && (*(insert_ptr + 1) == '\"' || *(insert_ptr + 1) == '\\'))
            ++insert_ptr;

        string_push_back(real_insert_str, *insert_ptr);
        ++insert_ptr;
    }

    string_delete(real_insert_str);
    free(real_insert_str);

    return 1;
}


void edit_string(string_t* str, cartesian_tree_t** data_tree_ptr, char* second_word)
{
	cartesian_tree_t* data_tree = *data_tree_ptr;
    size_t num1_val = 0;
    size_t num2_val = 0;
    string_t* edit_str = NULL;
    string_t* insert_str = NULL;
    char* key = get_word_in_next_pos(second_word);
    char* buff_smb_ptr = 0;
    char* first_index = 0;
    char* second_index = 0;
    char* edit_smb_ptr = 0;
    char edit_smb = 0;

    action_t mode = 0;

    if(*key++ == '-')
    {
        if(*key == 'd')
            mode = DELETE;
        else if(*key == 'e')
            mode = EDIT;
        else if(*key == 'p')
            mode = PASTE;
        else
        {
            fprintf(stderr, "Please input correct key: \"-d\", \"-e\" or \"-p\"\n");
            return;
        }
    }
    else
    {
         fprintf(stderr, "Please input correct key: \"-d\", \"-e\" or \"-p\"\n");
                return;
    }

    first_index = get_word_in_next_pos(key);

    if(*first_index == '\0')
    {
        fprintf(stderr, "Please input correct number of page\n");
        return;
    }

     second_index = get_word_in_next_pos(first_index);

    if(*second_index == '\0')
    {
        fprintf(stderr, "Please input correct number of position\n");
        return;
    }

    if(mode != DELETE)
    {
        edit_smb_ptr = get_word_in_next_pos(second_index);
		buff_smb_ptr = edit_smb_ptr;

        if(*edit_smb_ptr++ == '\'' && *edit_smb_ptr++ == '\\' && *edit_smb_ptr++ == 'n' && *edit_smb_ptr == '\'')
        {
            edit_smb = '\n';
        }

        else
        {
            edit_smb_ptr = buff_smb_ptr;
            if(*edit_smb_ptr++ != '\'' || *edit_smb_ptr++ == '\0' || *edit_smb_ptr != '\'')
            {
                fprintf(stderr, "Please input symbol for edit\n");
                return;
            }

            edit_smb = *(--edit_smb_ptr);
        }
    }


    if(get_index(&num1_val, first_index) && get_index(&num2_val, second_index))
    {
        if(num1_val > 0 && num1_val <= data_tree->size)
        {
            cart_tree_access(data_tree_ptr, num1_val - 1, &edit_str);

            if(edit_str == NULL)
            {
                fprintf(stderr, "Can't do this operation\n");
                return;
            }

            --num2_val;
           if(num2_val >= 0 && ((mode == PASTE &&  num2_val <= edit_str->size) || num2_val < edit_str->size))      
            {
                if(mode == EDIT)
                {
                    if(edit_smb != '\n')
                        *(edit_str->data + num2_val) = edit_smb;
                    else
                    {
                        string_construct(&insert_str);
                        string_big_push(insert_str, edit_str->data + num2_val + 1, edit_str->size - num2_val - 1);
                        string_big_remove(edit_str, edit_str->data + num2_val, edit_str->size - num2_val);
                        cart_tree_insert(&data_tree, insert_str, num1_val);
                        *data_tree_ptr = data_tree;
                    }

                    return;
                }
                
                if(mode == PASTE)
                {
                    if(edit_smb != '\n')
                        string_insert(edit_str, edit_smb, num2_val);
                    else
                    {
                        string_construct(&insert_str);
                        string_big_push(insert_str, edit_str->data + num2_val, edit_str->size - num2_val);
                        string_big_remove(edit_str, edit_str->data + num2_val, edit_str->size - num2_val);
                        cart_tree_insert(&data_tree, insert_str, num1_val);
                        *data_tree_ptr = data_tree;
                    }

                    return;
                }

                if(mode == DELETE)
                {
                    string_remove(edit_str, num2_val);
                    return;
                }
            }
        }
    }
           
    fprintf(stderr, "Please input correct number of page and position\n");
    return;
}






