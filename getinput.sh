
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


if [ -z "$1$AOC_COOKIE" ]; then
	echo "USAGE: getinput.sh [SESSION_COOKIE]"
	echo '  Alternative: `export AOC_COOKIE=VALUE`'
	exit 1
fi

if [ ! -z "$1" ]; then
	curl -H "cookie: session=$1" https://adventofcode.com/2019/day/5/input > input.txt
else
	curl -H "cookie: session=$AOC_COOKIE" https://adventofcode.com/2019/day/5/input > input.txt
fi

# curl -H "cookie: session=$1" https://adventofcode.com/2019/day/5/input | head -c -1 > input.txt
