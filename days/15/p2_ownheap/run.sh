ARG=input.txt

if [ "$1" == "debug" ]; then
	make debug > /dev/null && ./debug.out
elif [ "$1" == "fsanitize" ]; then
	make fsanitize > /dev/null && ./fsanitize.out
elif [ -z "$1" ]; then
	make > /dev/null && ./a.out $ARG
else
	make > /dev/null && ./a.out $@
fi
