test_arraylist:
	gcc -g -w -o target/tests_arraylist tests/arraylist.* helpers/arraylist.* helpers/memory.* && ./target/tests_arraylist

test_hashmap:
	gcc -g -w -o target/tests_hashmap tests/hashmap.* helpers/* && ./target/tests_hashmap

compile_all:
	./build_all.sh
