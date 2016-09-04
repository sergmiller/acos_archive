#include <stdlib.h>
#include <stdio.h>

int main(int argc,char ** argv)
{
    char c;
    int i;

    FILE *fp;

    if(argc != 2)
    {
        printf("CODE_ERROR_1: uncorrect quantity of arguments");
        exit(1);
    }

    if(!(fp = fopen(argv[1],"r+")))
    {
        printf("CODE_ERROR_2: can't open file");
        exit(2);
    }

    i = 0;

    while(!feof(fp))
    {
        if(fgetc(fp) == '.')
        {
            fseek(fp,i,SEEK_SET);
            fputc(',',fp);
        }
        ++i;
    }
    fclose(fp);
    return 0;
}
