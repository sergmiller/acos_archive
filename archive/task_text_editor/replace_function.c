#include "replace_function.h"
#include "cartesian_tree.h"
#include "string_for_text_editor.h"
#include "parser.h"

short replace_func(string_t* str, cartesian_tree_t** data_tree_ptr, char* first_word, char* second_word)
{
    /**get number of first deleted string**/
    if(*data_tree_ptr == NULL)
    {
        return 1;
    }
    else{
        cartesian_tree_t* data_tree = *data_tree_ptr;
        cartesian_tree_t* list_begin = NULL;
        cartesian_tree_t* list_end = NULL;
        cartesian_tree_t* buff_tree = NULL;

        data_warehouse_t insert_pack_data = {NULL, NULL, NULL};
        data_warehouse_t replaced_pack_data = {NULL, NULL, NULL};
        size_t num1_val = 0;
        size_t num2_val = 0;
        size_t range = 0;

        char* end_str = NULL;
        char* start_str = second_word;

        int flg;

        short indissoluble = 1;/*строка без переносов*/
        short replace_begin =  0; /*вставка в начало*/
        short replace_end = 0;/*вставка в конец*/

        string_t* buff_str = NULL;

        string_t* replaced_str = NULL;
        string_t* inserted_str = NULL;

        string_construct(&replaced_str);
        string_construct(&inserted_str);

        if(*second_word != '\"' && *second_word != '\0' && *second_word != '^' && *second_word != '$')
        {
            flg = total_parse_2index(&num1_val, &num2_val, first_word, *data_tree_ptr);
            if(!flg) 
                return 1;

            start_str = get_word_in_next_pos(get_word_in_next_pos(start_str));  
        }
        else
        {
            num1_val = 1;
            num2_val = data_tree->size;
        }

        if(*start_str != '\"' && *start_str != '\0' && *start_str != '^' && *start_str != '$')
        {
            fprintf(stderr, "Please input correct replaced string.\n");
            return 1;
        }

        /*заменяемая строка вводится следующей строкой*/
        if(*start_str == '\0')
        {
            string_construct(&str);
            if(get_next_string(str, stdin))
                    return 0;

            start_str = str->data;

            if(*(str->data) == ' ')
                start_str = get_word_in_next_pos(str->data);


            if(*start_str != '\"' && *second_word != '^' && *second_word != '$')
            {
                fprintf(stderr, "Please input correct replaced string.\n");
                return 1;
            }
        }
        
        if(*start_str == '\"')
        {
            end_str = ++start_str;
            while(*end_str != '\"')
            {
                if(*end_str == '\\')
                    ++end_str;

                if(*end_str == '\0')
                {
                    fprintf(stderr, "Please input correct replaced string.\n");
                    return 1;
                }

                string_push_back(replaced_str, *end_str);
                ++end_str;
            }

            if(*(end_str + 1) != '\0' && *(end_str + 1) != ' ')
            {
                fprintf(stderr, "Please input correct replaced string.\n");
                return 1;
            }

            if(end_str == start_str)
            {
                fprintf(stderr, "Can't replace empty string.\nPlease input correct string.\n");
                return 1;
            }
        }
        else
        {
            end_str = start_str;
            if(*start_str == '^' && (*(start_str + 1) == ' ' || *(start_str + 1) == '\0'))
                replace_begin = 1;
            else if(*start_str == '$' && (*(start_str + 1) == ' ' || *(start_str + 1) == '\0'))
                replace_end = 1;
            else
            {
                fprintf(stderr, "Please use correct params for replace.\n");
                return 1;
            }
        }

        start_str = get_word_in_next_pos(end_str);
        
        /*****************************************input the replacement string*****************************************/
        if(*start_str != '\"' && *start_str != '\0')
        {
            fprintf(stderr, "Please input correct the replacement string.\n");
            return 1;
        }

        /*заменяющая строка вводится следующей строкой*/
        if(*start_str == '\0')
        {
            string_construct(&str);
            if(get_next_string(str, stdin))
                    return 0;

            start_str = str->data;

            if(*(str->data) == ' ')
                start_str = get_word_in_next_pos(str->data);

            if(*start_str != '\"')
            {
                fprintf(stderr, "Please input correct the replacement string.\n");
                return 1;
            }
        }
        
        end_str = ++start_str;
        while(*end_str != '\"')
        {
            if(*end_str == '\\')
            {
                if(*(end_str + 1) != 'n')
                    ++end_str;
                else
                {
                    buff_str = NULL;
                    buff_tree = NULL;
                    string_construct(&buff_str);
                    string_big_push(buff_str, inserted_str->data + range, inserted_str->size - range);
                    cart_tree_construct(&buff_tree, buff_str);
                    range = inserted_str->size;

                    if(indissoluble)
                    {
                        indissoluble = 0;
                        list_begin = buff_tree;
                        list_end = list_begin;
                    }
                    else
                    {
                        buff_tree->left = list_end;
                        list_end->right = buff_tree;
                        list_end = buff_tree;
                    }

                    end_str += 2;
                    continue;
                }
            }

            if(*end_str == '\0')
            {   
                fprintf(stderr, "Please input correct the replacement string.\n");
                return 1;
            }

            string_push_back(inserted_str, *end_str);
            ++end_str;
        }

        if(!indissoluble)
        {
            string_construct(&buff_str);
            string_big_push(buff_str, inserted_str->data + range, inserted_str->size - range);
            range = inserted_str->size;
            buff_tree = NULL;
            cart_tree_construct(&buff_tree, buff_str);
            buff_tree->left = list_end;
            list_end->right = buff_tree;
            list_end = buff_tree;
        }

        if(*(end_str + 1) != '\0' && *(end_str + 1) != ' ')
        {
            fprintf(stderr, "Please input correct the replacement string.\n");
            return 1;
        }

        insert_pack_data.str_ptr = inserted_str;
        insert_pack_data.list_begin = list_begin;
        insert_pack_data.list_end = list_end;

        replaced_pack_data.str_ptr = replaced_str;
        replaced_pack_data.list_begin = NULL;
        replaced_pack_data.list_end = NULL;

       
        if(replace_begin)
            cart_tree_universal_action_with_subtree(data_tree_ptr, num1_val, num2_val, REPLACE, &insert_pack_data, NULL);
        else
            if(replace_end)
                cart_tree_universal_action_with_subtree(data_tree_ptr, num1_val, num2_val, REPLACE, NULL, &insert_pack_data);
            else
                cart_tree_universal_action_with_subtree(data_tree_ptr, num1_val, num2_val, REPLACE, &replaced_pack_data, &insert_pack_data);
        
        string_delete(replaced_str);
        string_delete(inserted_str);
        free(replaced_str);
        free(inserted_str);
        
        return 1;
    }
}
