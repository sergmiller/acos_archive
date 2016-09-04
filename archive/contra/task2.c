#include <stdio.h>
#include <string.h>


int main(int argc, char** argv)
{
    FILE* fp;


    size_t size = sizeof(long int);
    size_t i;

    long int num, res;


    if(!(fp = fopen(argv[1], "rb")))
        return -1;


    while(!feof(fp))
    {
        if(read(fp,&num, size) != size)
            return -1;

        res = 0;

        for(i = 0;i < size;++i)
        {
            res = res << 1;
            res += num % 2;
            num = num >> 1;
        }

        write(fp,&res,size);
        fseek(fp,size,SEEK_SET);
    }
    fclose(fp);
    return 0;
}
