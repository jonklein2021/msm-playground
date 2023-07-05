all: libcurve libmsm
	mkdir -p build/bin/
	g++ src/bin/test.cpp -o test -I include -lcurve -lmsm -L build/lib
	mv test build/bin

libcurve:
	mkdir -p build/lib/
	mkdir -p build/objects
	g++ src/curve.cpp -c -I include
	ar rs libcurve.a curve.o
	mv *.o build/objects
	mv *.a build/lib

libmsm:
	mkdir -p build/lib/
	mkdir -p build/objects
	g++ src/msm.cpp -c -I include
	ar rs libmsm.a msm.o
	mv *.o build/objects
	mv *.a build/lib
	
clean:
	rm -rf build
	rm -f tests.out