void editor_print_pages(data_container ** data, long start_index)
{
    struct termios old_term,new_term;
    char ch;
    winsize window_size;  
    int is_exit = FALSE;
    long count_printed_real_strings = start_index;
    int balance = 0;
    int is_need_new_str = FALSE;
    cartesian_tree * list = NULL;
    cartesian_tree * end_list = NULL;
    cartesian_tree * begin_list = NULL;

    ioctl(1,TIOCGWINSZ,&window_size);
    tcgetattr( STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~( ICANON | ECHO );
    
    if(cartesian_size(*data) == 0)
    {
        fprintf(stderr, "\nhave no strings");
    }
    else
    {
        conv_tree_to_list(&begin_list, &end_list, data);
        list = begin_list;

        while(!is_exit)
        {   
            system("clear");
            balance = window_size.ws_row ;
            while(balance > 0 && list != NULL)
            {
                is_need_new_str = super_print_string(list->text,&window_size,&count_printed_real_strings, &balance);
                if(is_need_new_str)
                {
                    list = list->right;
                }
            }
            if(list == NULL) is_exit = TRUE;
            tcsetattr( STDIN_FILENO, TCSANOW, &new_term );
            while(TRUE)
            {
                ch = getchar();
                if(ch == ' ')
                {
                    tcsetattr( STDIN_FILENO, TCSANOW, &old_term );
                    break;
                }
                if(ch == 'q' || ch == 'Q')
                {
                    tcsetattr( STDIN_FILENO, TCSANOW, &old_term );
                    is_exit = TRUE;
                    break;
                }  
            }
        }
        conv_list_to_tree(&begin_list, &end_list, data);
    }
}
