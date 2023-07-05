# msm-playground

## Build Instructions
```
make
./build/bin/test
```
OR
```
chmod 700 test.sh
./test.sh
```

## Summary
This is an attempt at a serial implementation of pippenger's algorithm for solving the Multi-Scalar Multiplication (MSM) problem. The goal of this project is to better understand elliptic curve arithmetic and the anatomy of MSM algorithms. Currently, there are several shortcomings, such as the sloppy organization of classes, the lack of a finite field for our elliptic curve to live on, and the lack of correctness between our implementation of pippenger's algorithm and our double-and-add algorithm. In the future, I have plans to clean everything up, but for now it remains a record of disheveled ideas and a playground for curiosity.
