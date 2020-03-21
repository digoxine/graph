n_nodes=$1
n_clusters=$2
p=500
q=1

classification="../Datas/classification.txt"
if [[ -f $classification ]]
then
    rm -rf $classification
fi
touch $classification
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
    fi
    touch $res_jaccard
    
    `./jaccard $tree > $res_jaccard`

    label_propagation="../Datas/label_propagation_res/label_propagation_"
    label_propagation+=$n_nodes
    label_propagation+="_p_"
    label_propagation+=`echo "scale=3; $p/1000"|bc`
    label_propagation+="_q_"
    label_propagation+=`echo "scale=3; $q/1000"|bc`
    label_propagation+="_nodes.res"
    if [[ -f $label_propagation ]]
    then
	rm -rf $label_propagation
    fi
    touch $label_propagation
    
    `./label_propagation $tree > $label_propagation`

    louvain="../Datas/label_propagation_res/louvain_"
    louvain+=$n_nodes
    louvain+="_p_"
    louvain+=`echo "scale=3; $p/1000"|bc`
    louvain+="_q_"
    louvain+=`echo "scale=3; $q/1000"|bc`
    label_louvain+="_nodes.res"
    if [[ -f $louvain ]]
    then
	rm -rf $louvain
    fi
    touch $louvain
    
    `./louvain $tree > $louvain`

    
    echo `echo "scale=3; $p/1000"|bc` `echo "scale=3; $q/1000"|bc` `./classification_score $n_clusters $res_jaccard` ` ./classification_score $n_clusters $label_propagation` `./classification_score $n_clusters $louvain`>> $classification
    
    ((q+=1))
done
