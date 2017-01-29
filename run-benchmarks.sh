#!/bin/bash
# TODO - needs to be rewritten to reduce copy'n'paste reuse
echo Running gimbatuluk benchmarks

echo Input size 1500000 12 matches 8 PCIe lanes
echo
./simple-benchmark-compact -s 1500000
./simple-benchmark-compact -s 1500000
./simple-benchmark-compact -s 1500000
./simple-benchmark-compact -s 1500000
echo
echo
./simple-benchmark-threaded-compact -s 1500000 -T 2
./simple-benchmark-threaded-compact -s 1500000 -T 2
./simple-benchmark-threaded-compact -s 1500000 -T 2
./simple-benchmark-threaded-compact -s 1500000 -T 2
echo
echo
./simple-benchmark-threaded-compact -s 1500000 -T 3
./simple-benchmark-threaded-compact -s 1500000 -T 3
./simple-benchmark-threaded-compact -s 1500000 -T 3
./simple-benchmark-threaded-compact -s 1500000 -T 3
echo
echo
./simple-benchmark-threaded-compact -s 1500000 -T 4
./simple-benchmark-threaded-compact -s 1500000 -T 4
./simple-benchmark-threaded-compact -s 1500000 -T 4
./simple-benchmark-threaded-compact -s 1500000 -T 4
echo
echo
echo Input size 1500000 12 matches 16 PCIe lanes
echo
./simple-benchmark-compact -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -s 1500000 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -s 1500000 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 4953699 12 matches 8 PCIe lanes
echo
./simple-benchmark-compact -s 4953699
./simple-benchmark-compact -s 4953699
./simple-benchmark-compact -s 4953699
./simple-benchmark-compact -s 4953699
echo
echo
./simple-benchmark-threaded-compact -s 4953699 -T 2
./simple-benchmark-threaded-compact -s 4953699 -T 2
./simple-benchmark-threaded-compact -s 4953699 -T 2
./simple-benchmark-threaded-compact -s 4953699 -T 2
echo
echo
./simple-benchmark-threaded-compact -s 4953699 -T 3
./simple-benchmark-threaded-compact -s 4953699 -T 3
./simple-benchmark-threaded-compact -s 4953699 -T 3
./simple-benchmark-threaded-compact -s 4953699 -T 3
echo
echo
./simple-benchmark-threaded-compact -s 4953699 -T 4
./simple-benchmark-threaded-compact -s 4953699 -T 4
./simple-benchmark-threaded-compact -s 4953699 -T 4
./simple-benchmark-threaded-compact -s 4953699 -T 4
echo
echo
echo Input size 4953699 12 matches 16 PCIe lanes
echo
./simple-benchmark-compact -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -s 4953699 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -s 4953699 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 1500000 479829 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t words -s 1500000 -L 12
./simple-benchmark-compact -t words -s 1500000 -L 12
./simple-benchmark-compact -t words -s 1500000 -L 12
./simple-benchmark-compact -t words -s 1500000 -L 12
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -T 4
echo
echo
echo Input size 1500000 479829 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 12 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 4953699 479829 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t words -L 12
./simple-benchmark-compact -t words -L 12
./simple-benchmark-compact -t words -L 12
./simple-benchmark-compact -t words -L 12
echo
echo
./simple-benchmark-threaded-compact -t words -L 12 -T 2
./simple-benchmark-threaded-compact -t words -L 12 -T 2
./simple-benchmark-threaded-compact -t words -L 12 -T 2
./simple-benchmark-threaded-compact -t words -L 12 -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -L 12 -T 3
./simple-benchmark-threaded-compact -t words -L 12 -T 3
./simple-benchmark-threaded-compact -t words -L 12 -T 3
./simple-benchmark-threaded-compact -t words -L 12 -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -L 12 -T 4
./simple-benchmark-threaded-compact -t words -L 12 -T 4
./simple-benchmark-threaded-compact -t words -L 12 -T 4
./simple-benchmark-threaded-compact -t words -L 12 -T 4
echo
echo
echo Input size 4953699 479829 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t words -L 12 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 12 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 12 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 12 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 12 -D OpenCL:GPU[1] -T 4
echo
################################################################################
echo
echo
echo Input size 1500000 256 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t test256 -s 1500000
./simple-benchmark-compact -t test256 -s 1500000
./simple-benchmark-compact -t test256 -s 1500000
./simple-benchmark-compact -t test256 -s 1500000
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 2
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 2
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 2
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 2
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 3
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 3
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 3
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 3
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 4
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 4
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 4
./simple-benchmark-threaded-compact -t test256 -s 1500000 -T 4
echo
echo
echo Input size 1500000 256 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t test256 -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test256 -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test256 -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test256 -s 1500000 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test256 -s 1500000 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 4953699 256 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t test256 -s 4953699
./simple-benchmark-compact -t test256 -s 4953699
./simple-benchmark-compact -t test256 -s 4953699
./simple-benchmark-compact -t test256 -s 4953699
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 2
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 2
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 2
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 2
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 3
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 3
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 3
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 3
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 4
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 4
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 4
./simple-benchmark-threaded-compact -t test256 -s 4953699 -T 4
echo
echo
echo Input size 4953699 256 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t test256 -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test256 -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test256 -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test256 -s 4953699 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test256 -s 4953699 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 1500000 479829 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t words -s 1500000 -L 256
./simple-benchmark-compact -t words -s 1500000 -L 256
./simple-benchmark-compact -t words -s 1500000 -L 256
./simple-benchmark-compact -t words -s 1500000 -L 256
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -T 4
echo
echo
echo Input size 1500000 479829 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 256 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 4953699 479829 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t words -L 256
./simple-benchmark-compact -t words -L 256
./simple-benchmark-compact -t words -L 256
./simple-benchmark-compact -t words -L 256
echo
echo
./simple-benchmark-threaded-compact -t words -L 256 -T 2
./simple-benchmark-threaded-compact -t words -L 256 -T 2
./simple-benchmark-threaded-compact -t words -L 256 -T 2
./simple-benchmark-threaded-compact -t words -L 256 -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -L 256 -T 3
./simple-benchmark-threaded-compact -t words -L 256 -T 3
./simple-benchmark-threaded-compact -t words -L 256 -T 3
./simple-benchmark-threaded-compact -t words -L 256 -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -L 256 -T 4
./simple-benchmark-threaded-compact -t words -L 256 -T 4
./simple-benchmark-threaded-compact -t words -L 256 -T 4
./simple-benchmark-threaded-compact -t words -L 256 -T 4
echo
echo
echo Input size 4953699 479829 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t words -L 256 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 256 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 256 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 256 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 256 -D OpenCL:GPU[1] -T 4
echo
################################################################################
echo
echo
echo Input size 1500000 1024 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t test1024 -s 1500000
./simple-benchmark-compact -t test1024 -s 1500000
./simple-benchmark-compact -t test1024 -s 1500000
./simple-benchmark-compact -t test1024 -s 1500000
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 2
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 2
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 2
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 2
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 3
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 3
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 3
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 3
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 4
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 4
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 4
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -T 4
echo
echo
echo Input size 1500000 1024 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t test1024 -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test1024 -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test1024 -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test1024 -s 1500000 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test1024 -s 1500000 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 4953699 1024 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t test1024 -s 4953699
./simple-benchmark-compact -t test1024 -s 4953699
./simple-benchmark-compact -t test1024 -s 4953699
./simple-benchmark-compact -t test1024 -s 4953699
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 2
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 2
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 2
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 2
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 3
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 3
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 3
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 3
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 4
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 4
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 4
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -T 4
echo
echo
echo Input size 4953699 1024 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t test1024 -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test1024 -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test1024 -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test1024 -s 4953699 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test1024 -s 4953699 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 1500000 479829 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t words -s 1500000 -L 1024
./simple-benchmark-compact -t words -s 1500000 -L 1024
./simple-benchmark-compact -t words -s 1500000 -L 1024
./simple-benchmark-compact -t words -s 1500000 -L 1024
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -T 4
echo
echo
echo Input size 1500000 479829 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 1024 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 4953699 479829 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t words -L 1024
./simple-benchmark-compact -t words -L 1024
./simple-benchmark-compact -t words -L 1024
./simple-benchmark-compact -t words -L 1024
echo
echo
./simple-benchmark-threaded-compact -t words -L 1024 -T 2
./simple-benchmark-threaded-compact -t words -L 1024 -T 2
./simple-benchmark-threaded-compact -t words -L 1024 -T 2
./simple-benchmark-threaded-compact -t words -L 1024 -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -L 1024 -T 3
./simple-benchmark-threaded-compact -t words -L 1024 -T 3
./simple-benchmark-threaded-compact -t words -L 1024 -T 3
./simple-benchmark-threaded-compact -t words -L 1024 -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -L 1024 -T 4
./simple-benchmark-threaded-compact -t words -L 1024 -T 4
./simple-benchmark-threaded-compact -t words -L 1024 -T 4
./simple-benchmark-threaded-compact -t words -L 1024 -T 4
echo
echo
echo Input size 4953699 479829 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t words -L 1024 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 1024 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 1024 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 1024 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 1024 -D OpenCL:GPU[1] -T 4
echo
################################################################################
echo
echo
echo Input size 1500000 16384 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t test16384 -s 1500000
./simple-benchmark-compact -t test16384 -s 1500000
./simple-benchmark-compact -t test16384 -s 1500000
./simple-benchmark-compact -t test16384 -s 1500000
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 2
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 2
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 2
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 2
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 3
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 3
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 3
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 3
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 4
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 4
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 4
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -T 4
echo
echo
echo Input size 1500000 16384 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t test16384 -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test16384 -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test16384 -s 1500000 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test16384 -s 1500000 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test16384 -s 1500000 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 4953699 16384 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t test16384 -s 4953699
./simple-benchmark-compact -t test16384 -s 4953699
./simple-benchmark-compact -t test16384 -s 4953699
./simple-benchmark-compact -t test16384 -s 4953699
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 2
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 2
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 2
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 2
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 3
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 3
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 3
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 3
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 4
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 4
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 4
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -T 4
echo
echo
echo Input size 4953699 16384 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t test16384 -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test16384 -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test16384 -s 4953699 -D OpenCL:GPU[1]
./simple-benchmark-compact -t test16384 -s 4953699 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t test16384 -s 4953699 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 1500000 479829 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t words -s 1500000 -L 16384
./simple-benchmark-compact -t words -s 1500000 -L 16384
./simple-benchmark-compact -t words -s 1500000 -L 16384
./simple-benchmark-compact -t words -s 1500000 -L 16384
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -T 4
echo
echo
echo Input size 1500000 479829 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -s 1500000 -L 16384 -D OpenCL:GPU[1] -T 4
echo
echo
echo Input size 4953699 479829 matches 8 PCIe lanes
echo
./simple-benchmark-compact -t words -L 16384
./simple-benchmark-compact -t words -L 16384
./simple-benchmark-compact -t words -L 16384
./simple-benchmark-compact -t words -L 16384
echo
echo
./simple-benchmark-threaded-compact -t words -L 16384 -T 2
./simple-benchmark-threaded-compact -t words -L 16384 -T 2
./simple-benchmark-threaded-compact -t words -L 16384 -T 2
./simple-benchmark-threaded-compact -t words -L 16384 -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -L 16384 -T 3
./simple-benchmark-threaded-compact -t words -L 16384 -T 3
./simple-benchmark-threaded-compact -t words -L 16384 -T 3
./simple-benchmark-threaded-compact -t words -L 16384 -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -L 16384 -T 4
./simple-benchmark-threaded-compact -t words -L 16384 -T 4
./simple-benchmark-threaded-compact -t words -L 16384 -T 4
./simple-benchmark-threaded-compact -t words -L 16384 -T 4
echo
echo
echo Input size 4953699 479829 matches 16 PCIe lanes
echo
./simple-benchmark-compact -t words -L 16384 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 16384 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 16384 -D OpenCL:GPU[1]
./simple-benchmark-compact -t words -L 16384 -D OpenCL:GPU[1]
echo
echo
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 2
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 2
echo
echo
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 3
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 3
echo
echo
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 4
./simple-benchmark-threaded-compact -t words -L 16384 -D OpenCL:GPU[1] -T 4
echo

