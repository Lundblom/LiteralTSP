#/bin/bash

SIZE=1048576
SIDE=1024

COUNTER=0

echo $SIZE
echo $SIDE


while(( COUNTER < SIZE )); do
	printf $((RANDOM % 30))
	printf " "

	if((COUNTER % SIDE == 0))
	then
		echo
	fi

	((COUNTER++))
done
