
# # # Firefox # # #
# 1. Go to https://adventofcode.com and log in
# 2. F12 or ctrl+shift+i (cmd+shift+i on MacOS)
# 3. Go to `Storage` tab
# 4. Cookies => adventofcode website
# 5. Copy session value
# # # # # # # # # #

# # # Chrome # # #
# 1. Go to https://adventofcode.com and log in
# 2. F12 or ctrl+shift+i (cmd+shift+i on MacOS)
# 3. Go to `Application` tab
# 4. Cookies => adventofcode website
# 5. Copy session value
# # # # # # # # # #

printUsage() {
	echo "USAGE: getinput.sh [DAY] [SESSION_COOKIE]"
	echo '  Alternative: `export AOC_COOKIE=VALUE`'
}

if [ -z "$1" ]; then
	printUsage
	exit 1
fi

if [ -z "$2$AOC_COOKIE" ]; then
	printUsage
	exit 1
fi

URL="https://adventofcode.com/2021/day/$1/input"
if [ ! -z "$2" ]; then
	COOKIE=$2
else
	COOKIE=$AOC_COOKIE
fi

curl -H "cookie: session=$COOKIE" "$URL" > input.txt
# curl -H "cookie: session=$COOKIE" "$URL" | head -c -1 > input.txt
