#include "my_print_pages.h"

short window_string_print(cartesian_tree_t** ptr, size_t* range_ptr, size_t* ind, short* log_size, unsigned short* cur_row_ptr, unsigned short cur_col)
{
    string_t* str;
    cartesian_tree_t* tree;
    size_t dop_size;
    size_t prev_range;

    char buff;

    while(1)
    {
        tree = *ptr;
        str = tree->text;

        if((long)(ind)/10 != ((long)(ind) - 1)/10)
            ++*log_size;

        if(!*range_ptr)
            dop_size = *log_size + 2;
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
            ++*ind;

            if(!*cur_row_ptr)
                return 0;
        }
        else
        {
            prev_range = *range_ptr;
            *range_ptr += (cur_col - dop_size);
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
	struct termios real_tm, std_tm;
    struct winsize cur_win_size;
    
    cartesian_tree_t* list_begin = NULL;
    cartesian_tree_t* list_end = NULL;
    cartesian_tree_t* ptr = NULL;

    char cnt;

    size_t ind = 1;
    size_t range = 0;
    
    unsigned short cur_col;
    unsigned short cur_row;

    short stop_print = 0;
    short log_size = 1;

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
        cur_col = cur_win_size.ws_col;
    	stop_print = window_string_print(&ptr, &range, &ind, &log_size, &cur_row, cur_col);
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

