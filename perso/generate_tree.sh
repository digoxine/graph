
if ! [[ $1 =~ ^[0-9]+$ ]]
then
    echo "This script takes the number of nodes of the tree you want to create"
else
    file="../Datas/graph_generated_toy/graph_"
    file+=$1
    file+="_p_"
    file+=$3
    file+="_q_"
    file+=$4
    file+="_nodes.txt"
    if [[ -f $file ]]
    then
	rm -rf $file
    else
	touch $file
    fi
    ./generateNode $1 $2 $3 $4> $file
    echo $file
fi
