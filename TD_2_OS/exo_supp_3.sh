
# tous les opérateurs marche sauf '*'
a="$1"
b="$2"
op="$3"
res=$(($a$op$b))

echo $res
