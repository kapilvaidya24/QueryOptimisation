$DIRECTORY="testcases"
g++ -std=c++11 -o algo algorithm/algorithm.cpp
g++ -o dphyp dphyp.cpp

for querytype in 'star' 'chain' 'clique' 
do
	for size in `seq 5 16`
	do
		for i in `seq 0 9`
		do
			# /usr/bin/time -f "%e" ./dphyp < "testcases/${querytype}_size_${size}_$i.txt" &> "testcases/output/dyphp_${querytype}_size_${size}_$i.out"
			# /usr/bin/time -f "%e" ./algo < "testcases/${querytype}_size_${size}_$i.txt" &> "testcases/output/algo_${querytype}_size_${size}_$i.out"
			/usr/bin/time -f "%e" ./dphyp < "$DIRECTORY/${querytype}_size_${size}_$i.txt" 1> "dphyp.out" 2> time1.txt
			/usr/bin/time -f "%e" ./algo < "$DIRECTORY/${querytype}_size_${size}_$i.txt" 1> "algorithm.out" 2> time2.txt
			diff <(echo $(cat dphyp.out| awk '{print $1}')) <(echo $(cat algorithm.out| awk '{print $1}')) &> /dev/null
			if [ $? -ne 0 ];
			then
				echo "Test Case $querytype size $size number $i differs"
			fi

			echo "$querytype $size $i $(cat dphyp.out) $(cat algorithm.out) $(cat time1.txt) $(cat time2.txt)" >> results.txt

		done
	done
done
