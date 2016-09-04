#include "string_for_text_editor.h"
#include <string.h>
#include "stdio.h"
#include "stdlib.h"


#define SIZE_OF_STRING_PARTS 300
const size_t MIN_FULL_SIZE_OF_STRING = 32;

void string_construct(string_t** str_ptr)
{
    string_t* str = *str_ptr;
    if(str)
        free(str->data);
    else
    {
	str = (string_t*)malloc(sizeof(string_t));
        if(str == NULL)
        {
            system("clear");
            printf("malloc_error, can't allocate %lub\n", sizeof(string_t));
            exit(1);
        }
	
        str->data = NULL;
    }
    /**default data array with MIN_FULL_SIZE_OF_STRING empty elements**/
    str->data = (char*)malloc(MIN_FULL_SIZE_OF_STRING * sizeof(char));
    
    if(str->data == NULL)
    {
        system("clear");
        printf("malloc_error, can't allocate %lub\n", MIN_FULL_SIZE_OF_STRING);
        exit(1);
    }

    /**work size**/
    str->size = 0;
    /**real size of allocated memory**/
    str->full_size = MIN_FULL_SIZE_OF_STRING;
    /**end of string counter**/
    str->data[0] = '\0';
    /**printf("%lu %lu\n",s->size,s->full_size);**/
    *str_ptr = str;
}

char* string_big_push(string_t* str, char* m_push, size_t m_size)
{
    char* test_data_ptr = NULL;
    size_t buff_size = 0;

    while(str->full_size <= str->size + m_size)
        str->full_size *= 2;

    buff_size = str->full_size;
    test_data_ptr = realloc(str->data, str->full_size);

    if(!test_data_ptr)
    {
        system("clear");
        fprintf(stderr, "CODE_ERROR_5: realloc_error, can't reallocate %lu b\n", buff_size * sizeof(char));
        exit(5);
    }

    str->data = test_data_ptr;
    buff_size = str->size;
    str->size += m_size;
    

    while(buff_size < str->size)
    {
        if(*m_push == '\n')
            break;

        str->data[buff_size] = *m_push;
        ++m_push;
        ++buff_size;
    }

    str->data[buff_size] = '\0';
    return m_push;
}

/*guaranted that there is (pos_ptr + size) in string*/
void string_big_remove(string_t* str, char* pos_ptr, size_t size)
{
    char* copy_pos = pos_ptr + size;
    
    do
    {
        *pos_ptr = *copy_pos;
        ++pos_ptr;
        ++copy_pos;
    }
    while(*copy_pos != '\0');

    if(size)
        while(size-- > 0)
            string_pop_back(str);

}
    
void string_big_insert(string_t* str, size_t pos, char* insert_str, size_t size)
{
    int i = size;

    char* test_data_ptr = NULL;
    size_t buff_size = 0;

    while(str->full_size <= str->size + size)
        str->full_size *= 2;

    buff_size = str->full_size;
    test_data_ptr = realloc(str->data, str->full_size);

    if(!test_data_ptr)
    {
        system("clear");
        fprintf(stderr, "CODE_ERROR_5: realloc_error, can't reallocate %lu b\n", buff_size * sizeof(char));
        exit(5);
    }

    str->data = test_data_ptr;
    i = str->size;
    str->size += size;

    for(;i > pos; --i)
    {
        str->data[i + size - 1] = str->data[i - 1];
    }

    i = size;

    if(i)
    {
        while(i-- > 0)
        {
            str->data[pos + i] = insert_str[i];
        }
    }
}

void string_push_back(string_t* str, char chr)
{
    char* test_data_ptr = NULL;
    size_t full_size = 0;
   /* printf("before:  size: %lu, full_size: %lu\n", str->size, str->full_size);*/
    /**increasing size**/
    ++str->size;
    /**unusual case of adding the element*/
    if(str->size >= str->full_size)
    {
        /**reallocation of memory when storage is full**/
        str->full_size *= 2;

        full_size = str->full_size;
        test_data_ptr = (char*)realloc(str->data, str->full_size);

        if(!test_data_ptr)
        {
            system("clear");
            printf("realloc_error: can't allocate %lu b\n", full_size);
            exit(2);
        }

        str->data = test_data_ptr;
    }
    
    /**insert last element**/
    str->data[str->size - 1] = chr;
    str->data[str->size] = 0;
   /* printf("after:  size: %lu, full_size: %lu\n", str->size, str->full_size);*/
}

/**edit element in index position**/
void string_assign(string_t* str, char chr, size_t index)
{
    *(str->data + index) = chr;
}

void string_insert(string_t* str, char chr, size_t index)
{
    size_t i = str->size;
    string_push_back(str, '0');
    /**shift of elements from [index,size) in right**/
    for(;i > index; --i)
        *(str->data + i) = *(str->data + i - 1);

    string_assign(str, chr, index);
}

void string_pop_back(string_t* str)
{
    if(!str->size)
        return;
    else
    {
        char* test_data_ptr = NULL;
        size_t full_size = 0;
         /**decreasing size*/
        --str->size;
        /**reallocation of memory when size less than quarter of full_size
          *but full_size can not to be less then MIN_FULL_SIZE_OF_STRING**/
        if(str->size * 4 <= str->full_size && str->full_size > MIN_FULL_SIZE_OF_STRING)
        {
            str->full_size /= 2;

            full_size = str->full_size;
            test_data_ptr = (char*)realloc(str->data, str->full_size);

            if(!test_data_ptr)
            {
                system("clear");
                printf("realloc_error: can't allocate %lu b\n", full_size);
                exit(2);
            }

            str->data = test_data_ptr;
        }
        /*remove last element*/
        *(str->data + str->size) = '\0';
    }
}

int string_scan_braces(int* deep_level, string_t* str)
{
    int err = 0;
    char* scan_ptr = str->data;

    while(*scan_ptr != '\0')
    {
        if(*scan_ptr == '{')
            ++*deep_level;
            
        if(*scan_ptr == '}' && --*deep_level < 0)
        {
            err = -1;
            break;
        }

        ++scan_ptr;
    }

    return err;
}

void string_delete_braces(int* deep_level, string_t* str)
{
    char* del_begin = str->data;
    size_t del_size = 0;
    char* scan_ptr = str->data;

    while(*scan_ptr != '\0')
    { 
        if(*scan_ptr == '{')
        {
            if(*deep_level == 0)
            {
                del_begin = scan_ptr;
                del_size = 0;
            }

            ++*deep_level;
        }

        if(*deep_level)
            ++del_size;
       
        if(*scan_ptr == '}')
        {
            --*deep_level;

            if(*deep_level == 0)
            {
                string_big_remove(str, del_begin, del_size);
                scan_ptr -= (del_size);
                del_size = 0;
            }
        }

        ++scan_ptr;
    } 

    if(*deep_level)
    {
        string_big_remove(str, del_begin, del_size);
    }
}

void string_remove(string_t* str, size_t index)
{
    size_t i = index + 1;
    /**shift of elements from [index,size) in left*/
    for(;i < str->size;++i)
        *(str->data + i - 1) = *(str->data + i);

    string_pop_back(str);
}

void string_print(string_t* str, FILE* fp)
{
    if(str == NULL)
    {
        fprintf(fp, "string is empty\n");
        return;
    }

    fprintf(fp, "%s\n", str->data);
}

void string_delete(string_t* str)
{
    free(str->data);
}

int string_compare(string_t* str_pars, size_t shift, const  char* str_cmp)
{
    size_t str_cmp_size = strlen(str_cmp);
    char* cur_pars_ptr =  str_pars->data + shift;
    char* last_pars_ptr = cur_pars_ptr + str_pars->size;
    const char* last_ptr = str_cmp + str_cmp_size;

    if((str_pars->size - shift) >= str_cmp_size)
    {
        while(cur_pars_ptr < last_pars_ptr && str_cmp < last_ptr)
        {
            if(*cur_pars_ptr == *str_cmp  && *str_cmp == ' ')
            {
                while(*cur_pars_ptr == ' ' && cur_pars_ptr < last_pars_ptr)
                    ++cur_pars_ptr;

                while(*str_cmp == ' ' && str_cmp < last_ptr)
                    ++str_cmp;

                continue;
            }


            if(*cur_pars_ptr++ != *str_cmp++)
                return 0;
        }

        if(*cur_pars_ptr != ' ' && *cur_pars_ptr != '\0')
            return 0;
    }
    else
        return 0;

    return 1;
}

char* get_word_in_next_pos(char* res_ptr)
{
    if(res_ptr == NULL)
        return NULL;

    while(*res_ptr != ' ' && *res_ptr != '\0')
    {
        if(*res_ptr == '#')
            return res_ptr;
        ++res_ptr;
    }

    while(*res_ptr == ' ' && *res_ptr != '\0')
    {
        if(*res_ptr == '#')
            return res_ptr;
        ++res_ptr;
    }
    
        return res_ptr;
}

int get_next_string(string_t* input_string, FILE* fp)
{
    char buff[SIZE_OF_STRING_PARTS];
    char* flag_inp = NULL;

    memset(&buff, 0, SIZE_OF_STRING_PARTS);
    string_construct(&input_string);

    while(1)
    {
        /**get new part of string**/
        flag_inp = fgets(buff, SIZE_OF_STRING_PARTS, fp);
        /**reading error or end of input string if progrmam input is automatic**/
        if (!flag_inp)
        {
            if (feof (fp))
                return -1;
            else
            {   
                system("clear");
                fprintf (stderr, "CODE_ERROR_4: can't read input string\n");
                exit(4);
            }
        }
        /**connect current string with new part**/
        string_big_push(input_string, buff, strlen(buff));

        if (*(buff + strlen(buff) - 1) == '\n' || feof(fp))
        {
           return 0;
        }
    }
}
