#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#define BUFF_SIZE 10

int main(int argc, char** argv)
{
    unsigned int fd;
    char* err1 = "uncorrect quantity of argument\n";
    char* err2 = "cann't open file\n";
    char buff[BUFF_SIZE];
    size_t size;

    if(argc != 2)
    {
    	write(3, err1, strlen(err1));
    	return 1;
    }
 	
    fd = creat(argv[1], 0600);

    if(fd == -1)
    {
    	write(3, err2, strlen(err2));
    	return 2;
    }

    dup2(fd, 1);

    while( (size = read(0, buff, BUFF_SIZE)) >0 )
    {
        write(1, buff, size);
    }

    close(fd);

    return 0;
}
