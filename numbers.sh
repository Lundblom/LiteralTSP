#/bin/bash

SIZE=10000
SIDE=100

COUNTER=0

echo $SIZE
echo $SIDE


while(( COUNTER < SIZE )); do
	if(( RANDOM % 30 < 40))
	then
		printf "1 "
	else
		printf "0 "
	fi

	if((COUNTER % SIDE == 0))
	then
		echo
	fi

	((COUNTER++))
done
