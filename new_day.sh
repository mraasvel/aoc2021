exitError() {
	echo "Error: $1"
	exit 1
}

if [ -z "$1" ]
then
	exitError "Usage: new_day.sh [DayNumber]"
fi

DIR="./days/$1"
if [ -d "$DIR" ];
then
	exitError "Directory: $DIR exists"
fi

echo "Creating $DIR using base template"
mkdir -p $DIR
cp -r ./base/* $DIR
cp "./getinput.sh" $DIR
