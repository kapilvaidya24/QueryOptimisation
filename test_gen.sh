DIRECTORY="testcases"
if [ ! -d "$DIRECTORY" ]; then
	mkdir "$DIRECTORY"
fi
for querytype in 'star' 'chain' 'clique'
do
	for size in `seq 5 16`
	do
		for i in `seq 0 9`
		do
			python "query_gen_${querytype}.py" $((100*$i+$size)) $size > "$DIRECTORY/${querytype}_size_${size}_$i.txt"
		done
	done
done
