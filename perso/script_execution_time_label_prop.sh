TIMEFORMAT=%R
start=5000
end=50000
p=500
q=400
cpt=$start
n_clusters=8
execution_time="../Datas/executionTimeLabel.txt"
if [[ -f $execution_time ]]
then
    rm -rf $execution_time
fi
touch $execution_time
while ((cpt<end))
do
    tree=$(./generate_tree.sh $cpt $n_clusters `echo "scale=3; $p/1000"|bc` `echo "scale=3;$q/1000"|bc`)
    time_bgll=$( time ( ./../Community_BGLL_CPP/convert -i $tree -o graph.bin | ./../Community_BGLL_CPP/community graph.bin -l -1 2>/dev/null 1>&2 ) 2>&1)
    #./../Community_BGLL_CPP/convert -i grd_graph.txt -o graph.bin 
    #time_bgll=$( time ( ./../Community_BGLL_CPP/community graph.bin -l -1 2>/dev/null 1>&2 ) 2>&1)
    time_label=$( time ( ./label_propagation $tree&>/dev/null 1>&2 ) 2>&1)
    echo $cpt `echo "scale=3; $p/1000"|bc` `echo "scale=3;$q/1000"|bc` $time_bgll $time_label >> $execution_time
    #echo $time_label
    ((cpt+=1000))
done


