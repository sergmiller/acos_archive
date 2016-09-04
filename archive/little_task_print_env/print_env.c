#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc , char** argv, char** envp)
{
    FILE * fl;
    size_t i = 0;
 
    if (argc != 2)
    {
        fprintf(stderr, "CODE_ERROR_!: uncorrect quantity of agguments\n");
        exit(1);
    }   

    if (!(fl = fopen(argv[1] , "w")))
    {
        fprintf(stderr, "CODE_ERROR_2: can't open file\n");
        exit(2);
    } 
      
    for(;i < argc;++i)
    {
        fprintf(fl, "%s\n", *(argv + i));
    }

    i = 0;
    
    while (*(envp + i))
    {
        fprintf(fl, "%s\n",*(envp + i));
        ++i;
    }	            
    
    fclose(fl);

    return 0;
}
