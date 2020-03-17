
if [[ -f "$1" ]]; then
    rm -rf $file
else
    touch $file
fi
./jaccard $1> $file
