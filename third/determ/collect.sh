#!/bin/bash

rm -f SOBOL_stats_d HALTON_stats_d LCG_stats_d

for ((p = 2; p <= $#; p++ ))
do
	echo ${!p}

	declare -A data
	declare -A count

	for ((i = 1; i <= $1; i++ ))
	do
		echo "$i"
	
		sudo perf stat ./deterministic ${!p} 2> tmp
		sudo perf record -F max -q ./deterministic ${!p} > /dev/null 2>&1
		sudo perf report -F +overhead,symbol --stdio | tail -n +12 | head -n -5 | awk '{print $1$2$3}' | tr '%' ' ' > overhead
	
		while read line
		do
	     		func=$(echo "$line" | cut -d' ' -f2)	   
			perc=$(echo "$line" | cut -d' ' -f1)
	
			if [[ ! -v "data[$func]" ]] ; then	
				data[$func]="$perc"
				count[$func]="1"
			else
				data[$func]+="+$perc"
				count[$func]+="+1"
			fi
		done < overhead 
		
		secs=$(cat tmp | tail -n 6 | head -n 1 | tr -s ' ' | cut -d' ' -f2)

                echo "$i $secs" >> "${!p}_stats_d"

	done

	rm -f tmp

	for i in "${!data[@]}"
	do
		amount=$(echo "${count[$i]}" | bc)
		mean=$(echo "scale=5; $(echo "${data[$i]}" | bc)/$amount" | bc)

		if (( $(echo "$mean < 1" | bc -l) && $(echo "$mean > 0" | bc -l) )); then
			mean="0$mean"
		elif (( $(echo "$mean == 0" | bc -l) )); then
			mean="0.00000"
		fi

		echo "$i: $mean $amount" >> tmp
	done

	cat tmp | sort -g -k2,2 | column -t > ${!p}_percentage_d

	rm -f tmp overhead perf.data*

	unset data
	unset count
done
