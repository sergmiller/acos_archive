#include "my_print_pages.h"
#include "string_for_text_editor.h"
#include "cartesian_tree.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

unsigned short my_log(size_t index)
{
    unsigned short ans = 0;
    while(index)
    {
        index /= 10;
        ++ans;
    }
    return ans;
}

short window_string_print(cartesian_tree_t** ptr, size_t* range_ptr, size_t* ind, unsigned short* cur_row_ptr, unsigned short cur_col)
{
    string_t* str = NULL;
    cartesian_tree_t* tree = NULL;
    unsigned short dop_size = 0;
    size_t prev_range = 0;

    char buff = 0;

    while(1)
    {
        tree = *ptr;
        str = tree->text;

        /*printf("log: %us\n", my_log(*ind));*/

        if(!*range_ptr && ind)
            dop_size = my_log(*ind) + 1;
        else
            dop_size = 0;
       
        if( str->size - *range_ptr + dop_size <= cur_col)
        {
            if(dop_size)
                printf("%lu>%s\n", *ind, str->data + *range_ptr);
            else
                printf("%s\n", str->data + *range_ptr);

            --*cur_row_ptr;

            if(!tree->right)
                return 1;

            *range_ptr = 0;
            *ptr = tree->right;
            
            if(ind)
                ++*ind;

            if(!*cur_row_ptr)
                return 0;
        }
        else
        {
            prev_range = *range_ptr;
            *range_ptr += (cur_col - dop_size + 1);
            buff = *(str->data + *range_ptr); 
            *(str->data + *range_ptr) = '\0';
            if(dop_size)
                printf("%lu>%s\n", *ind, str->data + prev_range);
            else 
                printf("%s\n", str->data + prev_range);
            
            --*cur_row_ptr;
             *(str->data + *range_ptr) = buff; 

            if(!*cur_row_ptr)
                return 0;
        }
    }
}


void print_pages(cartesian_tree_t* tree, size_t start)
{
    struct stat buff_in;
    struct stat buff_out;

    fstat(0, &buff_in);
    fstat(1, &buff_out);

    if((!S_ISCHR(buff_out.st_mode) && !S_ISBLK(buff_out.st_mode)) || (!S_ISCHR(buff_in.st_mode) && !S_ISBLK(buff_in.st_mode)))
	{
        print_tree(tree, stdout);
    }
    else
    {
        struct termios real_tm, std_tm;
        struct winsize cur_win_size;
        
        cartesian_tree_t* list_begin = NULL;
        cartesian_tree_t* list_end = NULL;
        cartesian_tree_t* ptr = NULL;

        char cnt = 0;

        size_t ind = start;
        size_t range = 0;
        
        unsigned short cur_col = 0;
        unsigned short cur_row = 0;

        short stop_print = 0;

        ioctl(1, TIOCGWINSZ, &cur_win_size);

        tcgetattr(0, &std_tm);

        real_tm = std_tm;

        real_tm.c_lflag &= ~ECHO;
        real_tm.c_lflag &= ~ICANON;

        real_tm.c_cc[VTIME] = 0;
        real_tm.c_cc[VMIN] = 1;

        if(tree == NULL)
        {
        	printf("List of strings is empty.\n");
    		return;
        }
        
        conv_tree_to_list(&list_begin, &list_end, &tree);
        ptr = list_begin;

        cur_col = cur_win_size.ws_col;

        while(1)
        {
        	system("clear");
            cur_row = cur_win_size.ws_row - 1;
            cur_col = cur_win_size.ws_col - 1;
        	stop_print = window_string_print(&ptr, &range, (!ind ? NULL : &ind), &cur_row, cur_col);
        	tcsetattr(0, TCSANOW, &real_tm);
        	while(1)
        	{
        		cnt = getchar();

        		if(cnt == 'q' || cnt == 'Q')
        		{
        			tcsetattr(0, TCSANOW, &std_tm);
                    stop_print = 1;
                    break;
        		}

        		if(!stop_print && cnt == ' ')
        		{
        			tcsetattr(0, TCSANOW, &std_tm);
                    break;
        		}
        	}
        	if(stop_print)
        		break;
        }
        conv_list_to_tree(&list_begin, &list_end, &tree);   
    }
}

