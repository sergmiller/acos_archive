#include "string_for_text_editor.h"
#include <string.h>

const size_t MIN_FULL_SIZE_OF_STRING = 32;

void string_construct(string_t** str_ptr)
{
    string_t* str = *str_ptr;
    if(str)
        free(str->data);
    else
    {
        str = malloc(sizeof(string_t));
        str->data = NULL;
    }
    /**default data array with MIN_FULL_SIZE_OF_STRING empty elements**/
    if(!(str->data = (char*)malloc(MIN_FULL_SIZE_OF_STRING * sizeof(char))))
    {
        printf("malloc_error, can't allocate %lub\n", MIN_FULL_SIZE_OF_STRING);
        exit(1);
    }

    /**work size**/
    str->size = 0;
    /**real size of allocated memory**/
    str->full_size = MIN_FULL_SIZE_OF_STRING;
    /**end of string counter**/
    *str->data = '\0';
    /**printf("%lu %lu\n",s->size,s->full_size);**/
    *str_ptr = str;
}

char* string_big_push(string_t* str, char* m_push, size_t m_size)
{
    /**
    while(str->full_size < str->size + m_size)
    {
        str->full_size *= 2;
        if(!(str->data = realloc(str->data, str->full_size)))
        {
            fprintf(stderr, "CODE_ERROR_5: realloc_error, can't reallocate %lu b\n", str->full_size * sizeof(char));
            exit(5);
        }
    }
    
    if(!strcat(str->data, m_push - 1))
    {
        printf("FAIL");
    }**/

    size_t i = 0;
    while(i++ < m_size)
    {
        if(*m_push == '\n')
            break;

        string_push_back(str, *m_push);
        ++m_push;
    }
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

    if(i)
        while(i > 0)
        {
            string_push_back(str, '0');
            --i;
        }

    i = str->size - size;

    for(;i > pos; --i)
    {
        str->data[i + size - 1] = str->data[i - 1];
    }

    i = size;

    if(i)
        while(i-- > 0)
        {
            str->data[pos + i] = insert_str[i];
        }
}

void string_push_back(string_t* str, char chr)
{
   /* printf("before:  size: %lu, full_size: %lu\n", str->size, str->full_size);*/
    /**increasing size**/
    ++str->size;
    /**unusual case of adding the element*/
    if(str->size >= str->full_size)
    {
        /**reallocation of memory when storage is full**/
        str->full_size *= 2;
        if(!(str->data = (char*)realloc(str->data, str->full_size)))
        {
            printf("realloc_error: can't allocate %lu b\n", str->full_size);
            exit(2);
        }
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

     /**decreasing size*/
    --str->size;
    /**reallocation of memory when size less than quarter of full_size
      *but full_size can not to be less then MIN_FULL_SIZE_OF_STRING**/
    if(str->size * 4 <= str->full_size && str->full_size > MIN_FULL_SIZE_OF_STRING)
    {
        str->full_size /= 2;
        if(!(str->data = (char*)realloc(str->data, str->full_size)))
        {
            printf("realloc_error");
            exit(2);
        }
    }
    /*remove last element*/
    *(str->data + str->size) = '\0';
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
    /**
    size_t i = 0;
    if(str == NULL)
    {
        printf("string is empty\n");
        return;
    }
    all strings in struct have '\n' in their ends therfore printf newline isn't required
    for(;i < str->size;++i)
        printf("%c", *(str->data + i));**/
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
    char* flag_inp;

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

/********************************/
void string_replace(string_t* source_str,size_t position, string_t* replace_str, size_t size)
{
    int i,k;
    string_t new_string;
    string_t* buff_str;
    if(replace_str == NULL)
    {   
        new_string.data = NULL;
        buff_str = &new_string;
        string_construct(&buff_str);
        replace_str = &new_string;
    }

    if(size > replace_str->size)
    {
        for(i = position,k = 0;i<position + replace_str->size;++i,++k)
        {
            source_str->data[i] = replace_str->data[k];
        }
        for(;i<source_str->size;++i)
        {
            source_str->data[i] = source_str->data[i+size - replace_str->size];
        }
        for(i = replace_str->size; i <size;++i)
        {
            string_pop_back(source_str);
        }
    }
    else
    {
        for(i = 0; i< replace_str->size - size;++i)
        {
            string_push_back(source_str, '0');
        }
        for(i = position + replace_str->size; i < source_str->size; ++i)
        {
            source_str->data[i] = source_str->data[i - (replace_str->size - size)]; 
        }
        for(i = position,k = 0;i<position + replace_str->size;++i,++k)
        {
            source_str->data[i] = replace_str->data[k];
        }
    }
}
