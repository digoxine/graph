n_nodes=$1
n_clusters=$2
p=500
q=1
tmp_bin="../Datas/tmp.bin"
tmp_tree="../Datas/tmp.tree"

classification="../Datas/classification.txt"
if [[ -f $classification ]]
then
    rm -rf $classification
else
    touch $file
fi

if [[ -f $tmp_bin ]]
then
    rm -rf $tmp_bin
else
    touch $tmp_bin
fi

if [[ -f $tmp_tree ]]
then
    rm -rf $tmp_tree
else
    touch $tmp_tree
fi


while ((q<1000)) 
do
    tree=$(./generate_tree.sh $n_nodes $n_clusters `echo "scale=3; $p/1000"|bc` `echo "scale=3;$q/1000"|bc`)
    res_jaccard="../Datas/label_propagation_res/jaccard_label_"
    res_jaccard+=$n_nodes
    res_jaccard+="_p_"
    res_jaccard+=`echo "scale=3; $p/1000"|bc`
    res_jaccard+="_q_"
    res_jaccard+=`echo "scale=3; $q/1000"|bc`
    res_jaccard+="_nodes.res"
    if [[ -f $res_jaccard ]]
    then
	rm -rf $res_jaccard
    else
	touch $res_jaccard
    fi
    `./jaccard $tree > $res_jaccard`
    res_bgll="../Datas/bgll_res/bgll_label_"
    res_bgll+=$n_nodes
    res_bgll+="_p_"
    res_bgll+=`echo "scale=3; $p/1000"|bc`
    res_bgll+="_q_"
    res_bgll+=`echo "scale=3; $q/1000"|bc`
    res_bgll+="_nodes.res"
    if [[ -f $res_bgll ]]
    then
	rm -rf $res_bgll
    else
	touch $res_bgll
    fi        
    #./../Community_BGLL_CPP/convert -i $tree -o $tmp_bin
    #./../Community_BGLL_CPP/community $tmp_bin -l -1 > $tmp_tree
    echo `echo "scale=3; $p/1000"|bc` `echo "scale=3; $q/1000"|bc` `./classification_score $n_clusters $res_jaccard` >> $classification
    
    ((q+=1))
done
