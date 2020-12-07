if [[ "$1" == "" ]]; then
    echo "Usage: $0 <dayXX>" >&2
    exit 1
fi

mkdir $1

if [ ! -f $1/part1.c ]; then
    echo "int main() {}" > $1/part1.c
fi

if [ ! -f $1/part2.c ]; then
    echo "int main() {}" > $1/part2.c
fi
