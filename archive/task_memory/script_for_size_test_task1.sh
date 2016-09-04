#!/bin/bash

#start size test:
declare -i count=0
declare -a list_opt
list_opt=("a_O0" "a_O1" "a_O2" "a_O3" "a_os")

while [ $count -ne 5 ]
do
	declare -i array_size=2000000 
	declare -i step=500000
		
	echo "size step line gallop random sum" >> resalts_size_test_${list_opt[$count]}.csv
	while [ $array_size -lt 90000000 ]  
	do
		echo "run size test with size: $array_size, step: $step and optimization version: ${list_opt[$count]}" 
		./${list_opt[$count]} $array_size $step resalts_size_test_${list_opt[$count]}.csv
		let "array_size = array_size + 2000000"
	done
	./cg_p resalts_size_test_${list_opt[$count]}.csv  #программа для замены всех . на , в файле выходных данных
	let "count = count + 1"
done
exit 0
#end size test
