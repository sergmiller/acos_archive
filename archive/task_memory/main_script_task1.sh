#!/bin/bash
#start compilation
gcc -ansi -pedantic -Wall -O0 code_task1.c -o  a_O0
gcc -ansi -pedantic -Wall -O1 code_task1.c -o  a_O1
gcc -ansi -pedantic -Wall -O2 code_task1.c -o  a_O2
gcc -ansi -pedantic -Wall -O3 code_task1.c -o  a_O3
gcc -ansi -pedantic -Wall -os code_task1.c -o  a_os

gcc  change_points.c -o cg_p
#end compilation

#start run tests
#bash script_for_size_test_task1.sh
bash script_for_step_test_task1.sh
#end run tests
