#/bin/bash

SIZE=1000000
SIDE=1000

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
