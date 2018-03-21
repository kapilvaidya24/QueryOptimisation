DIRECTORY="testcases"
if [ ! -d "$DIRECTORY" ]; then
	mkdir "$DIRECTORY"
fi
for querytype in 'star' 'clique'
do
	for size in `seq 17 40`
	do
		for i in `seq 0 9`
		do
			python "query_gen_${querytype}.py" $((100*$i+$size)) $size > "$DIRECTORY/${querytype}_size_${size}_$i.txt"
		done
	done
done
