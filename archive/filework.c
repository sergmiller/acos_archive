#include "filework.h"

short my_read_and_open(char* fp_ptr, cartesian_tree_t** data_tree_ptr, char** last_file_ptr, const char* cur_inp, action_t action)
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
	    FILE* fp;

	    char* ptr = fp_ptr;
	    int flg = 0;
	    size_t fp_size = 0;
	    size_t index = 1;

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

	    if(!(fp = fopen(fp_ptr, cur_inp)))
	    {
	        fprintf(stderr, "CODE_ERROR_3: can't open for reading file.\n");
	        string_delete(str);
	        free(str);
	        return -3;
	    }

	    tree_delete(data_tree);

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
			*last_file_ptr = str->data;
	    }
	    else
	    	string_delete(str);

	    free(str);
	    fclose(fp);
	    return 1;
	}
}

short my_write(char* fp_ptr, cartesian_tree_t** data_tree_ptr, char** last_file_ptr, const char* cur_write)
{
 	/**get name a new file for writing**/
    char* last_file = *last_file_ptr;

    char cmd;
    size_t fp_size = 0;
    FILE* fp;
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

        fprintf(stderr, "fp_size: %lu\n", fp_size);

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
            cmd = getchar();
            if(cmd =='n' || cmd == 'N')
                return 1;
        }
    }
    
    *last_file_ptr = last_file;

    if(!(fp = fopen (last_file, cur_write)))
    {
        fprintf(stderr, "CODE_ERROR_3: can't open for writing existing file.\n");
        return -3;
    }

    print_tree(*data_tree_ptr, fp);
    fclose(fp);

    return 1;
}
