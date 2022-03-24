test: test.o libhashset.a
	g++ test.o -o tester -lhashset -llinkedlist -L. -Llib
	mkdir -p build/bin/release
	mkdir -p build/lib/release
	mkdir -p build/objects
	mv *.o build/objects
	mv *.a build/lib/release
	mv tester build/bin/release/test

test.o: test/test.cpp
	g++ test/test.cpp -c -I include -I lib
