start=300
end=5000
cpt=$start
n_clusters=$1
p=$2
q=$3
while((cpt<end))
do
    tree=`./generate_tree.sh $cpt $n_clusters $p $q`
    res_jaccard="../Datas/jaccard_label_"
    res_jaccard+=$cpt
    res_jaccard+="_nodes.res"
    if [[ -f $res_jaccard ]]
    then
	rm -rf $res_jaccard
    else
	touch $res_jaccard
    fi
    ./jaccard $tree > $res_jaccard
    ((cpt+=100))
done

