#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "conveer.h"
#include "string_for_text_editor.h"


int correct_next_symbol(char* smb)
{   
    if(smb == NULL)
        return -1;

    if(*smb == '\0' || *smb == ' ' || *smb == '#')
        return 0;

    return 1;
}


/**subscribe of general parse function**/
short parse(string_t* str)
{
    
    char*** argv = (char***)malloc(sizeof(char**) * 4);
    size_t* argc = (size_t*)malloc(sizeof(size_t*) * 4);
    size_t current_qunt = 0;
    char*** flg1 = NULL;
    char** flg2 = NULL;
    size_t* flg3 = NULL;
    char* range = NULL;
    char* scan = str->data;
    char* start_pos = NULL;
    char start_flg = 1;
    size_t real_size = 8;
    size_t argv_size = 1;
    size_t pipes_qunt = 0;

    if(*scan == ' ')
        scan = get_word_in_next_pos(str->data); 

    if(!strncmp(scan, "exit", 4) && !correct_next_symbol(scan + 4))
    {
        return 0;
    }


    while(1)   
    {
        if(*scan == ' ' || *scan == '\0')
        {
    
            if(*start_pos == '|')
            {
                ++pipes_qunt;

                if(real_size <= argv_size)
                {
                    real_size *= 2;
                    flg1 = realloc(argv, real_size * sizeof(char**));
                    flg3 = realloc(argc, real_size * sizeof(size_t));
                    if(!flg1 || !flg3)
                    {
                        fprintf(stderr, "CODE_ERROR: realloc_error, can't reallocate %lu b\n", real_size * (sizeof(char**) + sizeof(size_t)));
                        exit(1);
                    }
                    argv = flg1;
                    argc = flg3;
                }
                argv[argv_size] = malloc(4 * sizeof(char*));
                argc[argv_size - 1] = current_qunt;
                ++argv_size;
                current_qunt = 0;
            }
            else
            {
                if(current_qunt >= argc[argv_size - 1])
                {
                    if(argc[argv_size - 1] == 0)
                        argc[argv_size - 1] = 2;
                    else
                        argc[argv_size - 1] *= 2;
                    flg2 = realloc(argv[argv_size], argc[argv_size- 1] * sizeof(char*));
                    if(!flg2)
                    {
                        fprintf(stderr, "CODE_ERROR: realloc_error, can't reallocate %lu b\n", argc[argv_size - 1] * sizeof(char*));
                        exit(1);
                    }
                }
                argv[argv_size - 1] = flg2;
                argv[argv_size - 1][current_qunt] = start_pos;
                ++current_qunt;
            }

            range = scan;
            start_flg = 1;
            if(*scan == '\0')
                break;
            scan = get_word_in_next_pos(scan);  
            *range = '\0';        
        }
        else
        {
            if(start_flg == 1)
            {
                start_flg = 0;
                start_pos = scan;
            }
            ++scan;
        }
    }
    
    if(argv_size != real_size)
    {
        argv[argv_size] = NULL;
        argc[argv_size] = NULL;
    }



    conveer(argv, argc, argv_size);
/*
    if(strncmp(argv[0], "cat", 3) == 0)
    {
        conveer(argv, pipes_qunt);
    }*/
    /*
    if(*first_word != '\0' && *first != '#')
        fprintf(stderr, "\"%s\" - command not found\n", str->data);*/
    return 1;
}

