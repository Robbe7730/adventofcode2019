for f in day*; do
    gcc -lm -w -o target/${f}_part1 $f/part1.c helpers/*
    gcc -lm -w -o target/${f}_part2 $f/part2.c helpers/*
done