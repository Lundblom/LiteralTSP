#/bin/bash

SIZE=1048576
SIDE=1024

COUNTER=0


while(( COUNTER < SIZE )); do
	if(( RANDOM % 30 < 28))
	then
		printf 0,
	else
		printf 1,
	fi

	if((COUNTER % SIDE == 0))
	then
		echo
	fi

	((COUNTER++))
done
