#/bin/bash

SIZE=40000
SIDE=200

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
