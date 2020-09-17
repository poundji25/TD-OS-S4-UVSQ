
gcc -g mystery-code.c 

a="release";
b="debug";

if [ "$a" = "$1" ]
then
./a.out
elif [ "$b" = "$1" ]
then
gdb a.out
else
sum=0;
for i in `seq 1 $1`
do
    sum=$(($sum+$i))
done
echo "resultat : "
echo $sum
echo "erreur bro"
fi


