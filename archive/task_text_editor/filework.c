#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filework.h"
#include "cartesian_tree.h"
#include "string_for_text_editor.h"
#include "parser.h"

short my_read_and_open(char* fp_ptr, cartesian_tree_t** data_tree_ptr, char** last_file_ptr, action_t action)
{
	if(*fp_ptr == '\0')
    {
        fprintf(stderr, "Please input name of reading file\n");
        return 1;
    }
    else
    {
	    cartesian_tree_t* data_tree = *data_tree_ptr;
	    string_t* str = NULL;
	    FILE* fp = NULL;

	    char* ptr = fp_ptr;
        char* buff = NULL;
	    int flg = 0;

	    size_t index = 0;
        size_t fp_size = 0;

	    while(*ptr != ' ' && *ptr != '\0')
	    {
	        ++ptr;
	        if(*ptr == ' ')
	        {
	            *ptr = '\0';
	            break;
	        }
	    }

	    fp_size = (size_t)(ptr - fp_ptr) + 1;

	    if(!(fp = fopen(fp_ptr, "r+")))
	    {
	        fprintf(stderr, "CODE_ERROR_3: can't open for reading file.\nMaybe this file isn't exist or you don't have access rights to the file\n");
	        return -3;
	    }

	    tree_delete(data_tree);
        data_tree = NULL;

	    while(1)
	    {
	        string_construct(&str);
	        flg = get_next_string(str, fp);
	        if(flg == -1)
	            break;

	        cart_tree_insert(&data_tree, str, index);

	        ++index;
	    }

	    *data_tree_ptr = data_tree;
	    
	    if(action == OPEN)
	    {
	    	free(*last_file_ptr);
            buff = malloc(fp_size);

            if(!buff)
            {
                system("clear");
                printf("realloc_error: can't allocate %lu b\n", fp_size);
                exit(2);
            }

            *last_file_ptr = buff;
            
            memcpy(buff, fp_ptr, fp_size);
	    }
	    
        string_delete(str);
	    free(str);
	    fclose(fp);
	    return 1;
	}
}

short my_write(char* fp_ptr, cartesian_tree_t** data_tree_ptr, char** last_file_ptr)
{
 	/**get name a new file for writing**/
    char* last_file = *last_file_ptr;

    char cmd = 0;
    size_t fp_size = 0;
    FILE* fp = NULL;
    /** size_t i = 0;**/

    /**case if name of file wasn't input**/
    if(!last_file && fp_ptr == '\0')
    {
        fprintf(stderr, "Unknown file.\nPlease use \"help\", \"read\" or \"open\" file.\n");
        return 1;
    }

    if(*fp_ptr != '\0')
    {
        while( *(fp_ptr + fp_size) != ' ' && *(fp_ptr + fp_size) != '\0')
            ++fp_size;

        if(!(last_file = (char*)realloc(last_file, (fp_size + 1))))
        {
            fprintf(stderr, "CODE_ERROR_5: realloc_error, can't allocate %lu b\n", fp_size * sizeof(char));
            return -5;
        }

        *(last_file + fp_size) = '\0';

        strncpy(last_file, fp_ptr, fp_size);
    }
    else
    {
        if(!last_file)
        {
            fprintf(stderr, "Can't do this: don't get name of file and don't save previous name of file.\n");
            return 1;
        }
        else
        {
            fprintf(stdout, "Don't get name of file.\nwrite in previous saved file:\"%s\".(y/n):\n", last_file);
            while(1)
            {
                cmd = getchar();
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
    }
    
    *last_file_ptr = last_file;

    if(!(fp = fopen (last_file, "w")))
    {
        fprintf(stderr, "CODE_ERROR_3: can't open for writing existing file.\n");
        return -3;
    }

    print_tree(*data_tree_ptr, fp);
    fclose(fp);

    return 1;
}
