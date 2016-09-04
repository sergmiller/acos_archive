#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include "parser.h"
#include "string_for_text_editor.h"
#include "conveer.h"

int main(int argc, char** argv)
{
    /**string buffer**/
    string_t* input_string = NULL;

    /**clear window**/
    system("clear");

    /**case if unknown file*/
 
    while(1)
    {
        printf(">>notshel: ");
        string_construct(&input_string);

        if(get_next_string(input_string, stdin) == -1)
            break;

        /**if parse isn't 0 then input_string is "exit"**/
        if(!parse(input_string))
            break;
    }

    string_delete(input_string);
    free(input_string);

    return 0;
}
