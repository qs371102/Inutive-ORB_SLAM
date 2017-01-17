sudo service inuservice stop

i=3
while(($i > 0))
do
echo "wait "$i"s ..."
let "i--"
sleep 1
done 

sudo service inuservice start
