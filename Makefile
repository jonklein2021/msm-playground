all: libcurve libmsm
	mkdir -p build/not_bin/
	g++ src/not_bin/test.cpp -o test -I include -lcurve -lmsm -L build/lib
	mv test build/not_bin

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