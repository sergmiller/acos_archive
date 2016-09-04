#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include "filework.h"
#include "parser.h"
#include "string_for_text_editor.h"
#include "cartesian_tree.h"

int main(int argc, char** argv)
{
   
    FILE* fp = NULL;
    /*name of last opened file*/
    char* last_file = NULL;
    char* test_inp = NULL;
    /*size of name of last file*/
    size_t file_len = 0;

    /**latest input symbol**/
    cartesian_tree_t* data_tree = NULL;
    cartesian_tree_t* help_tree = NULL;
    size_t quant_str = 0;
    /**string buffer**/
    string_t* input_string = NULL;

    /**new random for generate priorities in treap*/
    srand(time(NULL));

    /**clear window**/
    system("clear");

    /**case if unknown file*/
    if(!argv[1])
    {
        printf("File is unknown. Create empty set of strings.\n");
    }
    else
    {
        if (!(fp = fopen(argv[1], "r+")))
        {
                printf("File doesn't exist.\n");
                if(!(fp = fopen(argv[1], "w")))
                {
                    system("clear");
                    fprintf(stderr,"CODE_ERROR_2: file can't be created.\n");
                    exit(2);
                }
                printf("File is created.\n");
        }
        else
        {
            /**read current file*/

            file_len = strlen(argv[1]);
            test_inp = (char*)malloc(file_len + 1);
            
            if(!test_inp)
            {
                system("clear");
                printf("realloc_error: can't allocate %lu b\n", file_len + 1);
                exit(2);
            }
            last_file = test_inp;

            strncpy(last_file, argv[1], file_len);
            printf("File is open\n");
            
            fp = fopen (last_file, "r+");

            if(!fp)
            {
                system("clear");
                fprintf(stderr, "CODE_ERROR_3: can't open existing file.\n");
                exit(3);
            }

            while (1)
            {

                string_construct(&input_string);
                if(get_next_string(input_string, fp) == -1)
                    break;

                cart_tree_insert(&data_tree, input_string, quant_str);
                ++quant_str;

            }
            /**close read file**/
            fclose(fp);
        }
    }
    /*print_tree(data_tree, stdout);
    printf("\n");*/
    
    my_read_and_open("help.txt", &help_tree, NULL, READ);

    while(1)
    {
        printf(">>editor: ");
        string_construct(&input_string);

        if(get_next_string(input_string, stdin) == -1)
            break;

        /**if parse isn't 0 then input_string is "exit"**/
        if(!parse(input_string, help_tree, &data_tree, &last_file))
            break;
    }


    tree_delete(data_tree);
    tree_delete(help_tree);
    string_delete(input_string);
    free(input_string);
    free(last_file);
    free(test_inp);
    system("clear");

    return 0;
}
