day="day$1"
part="part$2"

shift 2

gcc -g -O3 -lm -o target/$day"_"$part $day/$part.c helpers/* && ./target/$day"_"$part $@
