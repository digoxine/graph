nb_nodes=5000
nb_clusters=10
p=500
q=1
eval_score_label_louvain="../Datas/evalScoreLabelLouvain.txt"
if [[ -f $eval_score_label_louvain ]]
then
    rm -rf $eval_score_label_louvain
fi
touch $eval_score_label_louvain
tmp="temp.txt"
while ((q<1000))
do
    tree=$(./generate_tree.sh $nb_nodes $nb_clusters `echo "scale=3; $p/1000"|bc` `echo "scale=3;$q/1000"|bc`)
    $(./../Community_BGLL_CPP/convert -i $tree -o "graph.bin")
    ./../Community_BGLL_CPP/community $(echo "graph.bin") -l -1 > $(echo "graph.tree" )
    head -n -$nb_clusters graph.tree > temp2.txt ;mv temp2.txt graph.tree
    ./label_propagation $tree > $tmp
    echo `echo "scale=3; $p/1000"|bc` `echo "scale=3;$q/1000"|bc` `./classification_score $nb_clusters graph.tree` `./classification_score $nb_clusters $tmp` >> $eval_score_label_louvain
    ((q+=1))
done
if [[ -f "graph.bin" ]]
then
    rm graph.bin
fi
if [[ -f "graph.tree" ]]
then
    rm graph.tree
fi

if [[ -f $tmp ]]
then
    rm $tmp
fi

