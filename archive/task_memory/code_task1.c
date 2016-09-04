#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

struct test_res;
struct buff_struct; 

struct test_res Full_tests (size_t size_of_array , unsigned int step , short key);

void Random_init (unsigned int* Tested_Array , size_t size_of_array);
struct buff_struct Get_time (unsigned int** Indexation , size_t size_of_array);

void Generate_Line_Index   (unsigned int** Indexation , unsigned int* Tested_Array , size_t size_of_array);
void Generate_Gallop_Index (unsigned int** Indexation , unsigned int* Tested_Array , size_t size_of_array , unsigned int step);
void Generate_Random_Index (unsigned int** Indexation , unsigned int* Tested_Array , size_t size_of_array);

/*for save total sum or buffer_var*/
struct buff_struct
{
    float run_time;
    unsigned int buff;
};

/*for save test's results*/
struct 	test_res
{
    float time_of_line_test , time_of_gallop_test , time_of_random_test;
    unsigned int sum_ind;
};


/*main worked function*/
struct test_res Full_tests(size_t size_of_array , unsigned int step , short key)
{
    /*init objects of resault*/
    struct test_res Correct_Test_Answers;
    struct buff_struct result_test;
    unsigned int** Indexation;
    unsigned int* Tested_Array;
    unsigned int main_sum = 0;
    /*init random*/
    srand(time(NULL));

    /*init tested array*/
    Tested_Array = (unsigned int*)malloc(size_of_array * sizeof(unsigned int));
    Random_init (Tested_Array , size_of_array);

    /*init indexation*/
    Indexation = (unsigned int**)malloc(size_of_array * sizeof(unsigned int*));

    /*test with gallop access to memory*/
    if (key == 0 || key == 6)
    {
        Generate_Gallop_Index (Indexation , Tested_Array , size_of_array , step);
        result_test = Get_time (Indexation , size_of_array);
        main_sum += result_test.buff;
        Correct_Test_Answers.time_of_gallop_test = result_test.run_time;
    }
   
    /*create general line indexation for line test and for generate random*/ 
    if (key == 0 || key == 10 || key == 17)
    {
        Generate_Line_Index (Indexation , Tested_Array , size_of_array);
    }

    /*test with linear access*/
    if (key == 0 || key == 10)
    {
        result_test = Get_time (Indexation , size_of_array);
        main_sum += result_test.buff;
        Correct_Test_Answers.time_of_line_test = result_test.run_time;
    }

    /*random access*/
    if (key == 0 || key == 17)
    {
        Generate_Random_Index (Indexation , Tested_Array , size_of_array);
        result_test = Get_time (Indexation , size_of_array);
        main_sum += result_test.buff;
        Correct_Test_Answers.time_of_random_test = result_test.run_time;
    }
    
    /*main_sum is correct only then key = 'a'*/
    Correct_Test_Answers.sum_ind = main_sum / 3;
    free (Indexation);
    free (Tested_Array);
    return Correct_Test_Answers;
}

/*init random values in tested array*/
void Random_init (unsigned int* Tested_Array , size_t size_of_array)
{
    size_t i = 0;

    for (;i < size_of_array;++i)
    {
        *(Tested_Array + i) = rand();
    }
}

/**get time of run for all tests**/
struct buff_struct Get_time (unsigned int** Indexation , size_t size_of_array)
{
    struct buff_struct test_res;
    size_t i = 0;
    unsigned int buffer_var = 0;
    float start_time = clock();
   
    for(;i < size_of_array;++i)
    {
        buffer_var += **Indexation++;
    }

    test_res.run_time = (float) (clock() - start_time)/1000000;
    test_res.buff = buffer_var;
    return  test_res;
}

/*test-order with line access*/
void Generate_Line_Index (unsigned int** Indexation , unsigned int* Tested_Array , size_t size_of_array)
{
    size_t i = 0;

    for(;i < size_of_array;++i)
    {
        *Indexation++ = Tested_Array++;
    }
}

/*test-order with gallop access*/
void Generate_Gallop_Index (unsigned int** Indexation , unsigned int* Tested_Array , size_t size_of_array , unsigned int step)
{
    size_t i = 0;
    size_t j = 0;

    for(;i < step;++i)
    {
        j = i;

        for(;j < size_of_array;j += step)
        {
            *Indexation = Tested_Array + j;
            ++Indexation;
        }
    }
}

/*test-order with random access*/
void Generate_Random_Index (unsigned int** Indexation , unsigned int* Tested_Array , size_t size_of_array)
{
    /*line indexation from previous test used for generate random permutation*/
    size_t i = size_of_array;
    unsigned int* buff_index;
    size_t rand_pos = rand() % i;
   
    for(;i > 0;--i)
    {
        rand_pos = rand() % i;
        /*swap i and (rand() % i) elements*/    
        buff_index = *(Indexation + rand_pos);
        *(Indexation + rand_pos) = *(Indexation + (i - 1));
        *(Indexation + (i - 1)) = buff_index;
    }
}


int main(int argc, char ** argv)
{
    size_t size_of_array;
    unsigned int step;
    short key = 0;
    struct test_res ans;
    FILE * fp;

    /*program uses three arguments: size of array , step of access and name of output file.*/
    if (argc != 4 && argc != 5)/*there is use special param for run only one specific test*/
    {
        printf("CODE_ERROR_1: uncorrect quantity of arguments\n");
        exit(1);
    }

    size_of_array = (size_t) atoi(argv[1]);
    step = (unsigned int) atoi(argv[2]);
    

    if(!(fp = fopen(argv[3],"a+")))
    {
        printf("CODE_ERROR_2: can't open file\n");
        exit(2);
    }

    if (argc == 5)
    {
        key = atoi(argv[4]);
       
        /*key verification*/ 
        if (key != 0 && key != 10 && key != 6 && key != 17) /*it's magic keys*/
        {
            printf("CODE_ERROR_3: uncorrect key\n");
            exit(3);
        }		
    }

    ans = Full_tests(size_of_array , step , key);

    fprintf(fp , "%lu %u " , size_of_array , step);
    
    if (key == 0 || key == 10) 
    {
        fprintf(fp , "%f " , ans.time_of_line_test);
    }

    if (key == 0 || key == 6) 
    {
        fprintf(fp , "%f " , ans.time_of_gallop_test);
    }

    if (key == 0 || key == 17) 
    {
        fprintf(fp , "%f " , ans.time_of_random_test);
    }

    fprintf(fp , "%u\n" , ans.sum_ind); /*need to print total sum*/
    
    fclose(fp);

    return 0;
}





















