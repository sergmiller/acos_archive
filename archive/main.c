#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include "string_for_text_editor.h"
#include "cartesian_tree.h"
#include "parser.h"

#define SIZE_OF_STRING_PARTS 300

const char my_txt_read[] = {'r','+'};
const char my_bin_read[] = {'r','b','+'};
const char my_txt_write[] = {'w'};
const char my_bin_write[] = {'w','b'};
const char* my_txt_read_ptr = (char*)&my_txt_read;
const char* my_bin_read_ptr = (char*)&my_bin_read;
const char* my_txt_write_ptr = (char*)&my_txt_write;
const char* my_bin_write_ptr = (char*)&my_bin_write;

int main(int argc, char** argv)
{
    FILE* fp = NULL;
    /**struct winsize cur_size;**/
    /**get current window size**/
    /** int flag = ioctl(0, TIOCGWINSZ, &cur_size);**/ /**<-это пока не нужно**/

    /*name of last opened file*/
    char* last_file = NULL;
    /*type of file work with*/
    file_t f_type;
    char* cur_inp;
    char* cur_write;
    /*size of name of last file*/
    size_t file_len = 0;

    /**latest input symbol**/
    char* flag_inp;
    cartesian_tree_t* data_tree;
    size_t quant_str;
    short bin_count;
    /**param for constrcuct new string**/
    short key;
    /**string buffer**/
    string_t* input_string = NULL;
    /**buffer string for addition lines**/
    char buff[SIZE_OF_STRING_PARTS];

    key  = 0;
    bin_count = 0;
    quant_str = 0;
    data_tree = NULL;
    /**new random for generate priorities in treap*/
    srand(time(NULL));

    /**there is use special param for run only one specific test**/
    if (argc > 3)
    {
        fprintf(stderr, "CODE_ERROR_1: uncorrect quantity of arguments.\n");
        exit(1);
    }

    /**clear window**/
    system("clear");

    /**case if file is binary**/
    if (argc == 3 && (memcmp(argv[2], "-b\0", 3) || memcmp(argv[2], "-b ", 3)))
    {
        bin_count = 1;
        cur_inp = (char*)my_bin_read_ptr;
        cur_write = (char*)my_bin_write_ptr;
    }
    else
    {
        /**other file has txt format*/
        cur_inp = (char*)my_txt_read_ptr;
        cur_write = (char*)my_txt_write_ptr;
    }

    /**case if unknown file*/
    if(!argv[1])
    {
        printf("File is unknown. Create empty set of strings.\n");
    }
    else
    {
        if (!(fp = fopen(argv[1], cur_inp)))
        {
                printf("File doesn't exist.\n");
                if(!(fp = fopen(argv[1], cur_write)))
                {
                    fprintf(stderr,"CODE_ERROR_2: file can't be created.\n");
                    exit(2);
                }
                printf("File is created.\n");
        }
        else
        {
            /**read current file*/

            file_len = strlen(argv[1]);
            last_file = (char*)realloc(last_file, file_len + 1);
            strncpy(last_file, argv[1], file_len);
            printf("File is open\n");
            
            if(!(fp = fopen (last_file,"r")))
            {
                fprintf(stderr, "CODE_ERROR_3: can't open existing file.\n");
                exit(3);
            }

            while (1)
            {
                /**count for check start of new string*/
                if(!key)
                {
                    string_construct(&input_string);
                    printf("str: %p\n", input_string);
                    printf("OK1\n");
                    key = 1;
                }

                /**get new part of string from file*/
                flag_inp = fgets(buff, SIZE_OF_STRING_PARTS, fp);
                printf("OK2\n");
                /**check end of file or reading error*/
                if (!flag_inp)
                {
                    if (feof (fp))
                    {
                        printf ("File was read\n");
                        break;
                    }
                    else
                    {
                        fprintf (stderr, "CODE_ERROR_4: can't read from open file\n");
                        tree_delete(data_tree);
                        string_delete(input_string);
                        exit(4);
                    }
                }
                /**connect current string with new part*/
                string_big_push(input_string, buff, strlen(buff));

                if (*(buff + strlen(buff) - 1) == '\n' || bin_count)
                {
                    /**if reading is stop then insert this string in tree*/
                    cart_tree_insert(&data_tree, input_string, quant_str);
                    /**update counts**/
                    ++quant_str;
                    key = 0;
                }
            }
            /**close read file**/
            fclose(fp);
        }
    }

    /*print_tree(data_tree, stdout);
    printf("\n");*/

    key = 0;

    while(1)
    {
        /**count for check start of new string**/
        if(!key)
        {
            string_construct(&input_string);
            key = 1;
            printf(">>editor: ");
        }

        /**get new part of string**/
        flag_inp = fgets(buff, SIZE_OF_STRING_PARTS, stdin);
        /**reading error or end of input string if progrmam input is automatic**/
        if (!flag_inp)
        {
            if (feof (stdin))
            {
                printf ("input string was read\n");
                break;
            }
            else
            {
                fprintf (stderr, "CODE_ERROR_4: can't read input string\n");
                tree_delete(data_tree);
                string_delete(input_string);
                exit(4);
            }
        }
        /**connect current string with new part**/
        string_big_push(input_string, buff, strlen(buff));

        if (*(buff + strlen(buff) - 1) == '\n' || feof(stdin))
        {
            /**if parse isn't 0 then input_string is "exit"**/
            if(!parse(input_string, &data_tree, &last_file, cur_inp, cur_write))
                break;

            key = 0;
        }
    }


    tree_delete(data_tree);
    string_delete(input_string);

    return 0;
}
