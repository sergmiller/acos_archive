#!/bin/bash

#start step test:
declare -i count=0

declare -a list_opt
list_opt=("a_O0" "a_O1" "a_O2" "a_O3" "a_os")

while [ $count -ne 5 ]
do
	declare -i array_size=100000000 
	declare -i step=1000000

	echo "size step random buff_val" >> resalts_step_test_${list_opt[$count]}.csv	
	while [ $step -lt 50000000 ]  
	do
		echo "run step test with size: $array_size, step: $step and optimization version: ${list_opt[$count]}" 
		./${list_opt[$count]} $array_size $step resalts_step_test_${list_opt[$count]}.csv 6 #<-- gallop key used
		let "step = step + 1000000"
	done
	./cg_p resalts_step_test_${list_opt[$count]}.csv  #программа для замены всех . на , в файле выходных данных
	let "count = count + 1"
done
exit 0
#end step test
