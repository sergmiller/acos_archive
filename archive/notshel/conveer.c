#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "conveer.h"
#include "string_for_text_editor.h"

void conveer(char*** argv, size_t* argc, size_t argv_size)
{
    size_t pipes_qunt = argv_size - 1;
	int** channel = (int**)malloc(sizeof(int*) * pipes_qunt);
	int i;
	/*int out_file  = 0;
	unsigned int out_fd;*/
/*
	if(argv[2 * ][0][0] == '>' && argv[2 * pipes_qunt][0][1]== '>')
	{
		out_fd = creat(argv[3 + 2 * pipes_qunt], 0600);
		out_file = 1;
	}*/

	/*printf("%d\n", pipes_qunt);*/
	for(i = 0;i < pipes_qunt;++i)
	{
		channel[i] = (int*)malloc(sizeof(int) * 2);
		if(pipe(channel[i]))
		{
			fprintf(stderr, "CODE_ERROR: don't create channel\n");
			exit(2);
		}
	}
    
	for(i = 0;i <= pipes_qunt;++i)
	{
		if(!fork())
		{
			if(i != pipes_qunt)
				dup2(channel[i][1],1);

            if(i != 0)
            {
                dup2(channel[i-1][0],0);
			    close(channel[i-1][1]);
            }

            if(i != pipes_qunt)
			    close(channel[i][0]);

			execvp(argv[i][0], argv[i]);
			return;
		}
	}

	for(i = 0;i < pipes_qunt;++i)
	{
		close(channel[i][0]);
		close(channel[i][1]);
	}

	for(i = 0;i <= pipes_qunt;++i)
		wait(NULL);

	return;
}
    