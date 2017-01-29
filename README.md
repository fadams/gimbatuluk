![gimbatuluk](doc/gimbatuluk1.png)
# gimbatuluk
Gimbatuluk is a pattern matching accelerator written in C++11 and OpenCL.

The OpenCL Kernel is implemented using the Parallel Failureless Aho-Corasick algorithm with optional stream compaction of the result set.

If you are curious about the crazy name, it means "to find them all" in J.R.R. Tolkien's Black Speech. It is derived from the inscription upon the One Ring: “Ash nazg durbatulûk, ash nazg gimbatul, ash nazg thrakatulûk agh burzum-ishi krimpatul” meaning; One ring to rule them all, one ring to find them, one ring to bring them all and in the darkness bind them.

There is a detailed write-up of the design approach in the doc directory, the following is a basic summary.

The Aho-Corasick algorithm was first presented in 1975 by Alfred V. Aho and Margaret J. Corasick and is one of the most widely known and commonly adopted pattern scanning algorithms due to its property of matching all patterns in the target dictionary simultaneously.

The conventional Aho-Corasick algorithm can be parallelised effectively via an approach known as the Data Parallel Aho-Corasick (DPAC) algorithm, which partitions the sequence to be scanned into multiple (overlapping) segments and allocates each segment to a processing thread.

As the number of processing threads reaches one per character of the input sequence we can however take a radically different approach and dispense with the failure transitions altogether. This approach is known as the Parallel Failureless Aho-Corasick (PFAC) algorithm where each thread is only required to match the pattern that starts at its own thread starting position.

Gimbatuluk has only been tested on Linux and requires a C++11 compiler and OpenCL >= 1.2 to be installed. It has primarily been tested using Nvidia GTX Titan X GPUs, though it should work with any Nvidia GPU. It has also been tested with Intel's OpenCL CPU device however the implementation is currently sub-optimal on that device. It may work with AMD GPUs and the code tries to use AMD's 64 lane wavefront if it detects an AMD device, but the warp/wavefront optimisation code has only been tested on Nvidia's 32 lane warp.

To check whether OpenCL is correctly installed on your system run:
````
clinfo
````

To build Gimbatuluk:
````
cd gimbatuluk
mkdir build
cd build
cmake ..
make
````

Several simple example applications have been included, use the -h option for help. For example:
````
./simple-scan
````

by default loads the "words" dictionary and scans the text "the fat cat sat on the mat and acted like a prat", however:
````
./simple-scan -t test256
````
will scan the text file test256


To find how fast it can go there are a few simple benchmark programs, for example:
````
./simple-benchmark-threaded-compact -t test16384 -s 5000000 -T 2
````
will read the text file test16384, which has 16384 matching words, and will expand that into an input vector of 5MB it then performs a scan using two instances of the scanner to maximise data transfer throughput.


In general the limiting factor is likely to be PCIe bandwidth rather than Kernel performance so if you have 16xPCIe 3 lanes you should see the best system performance.

The API is relatively simple as may be seen from the main body of simple-scan as illustrated below.
````
// Read the text we want to scan into memory.
const auto input = textIsFile ? gimbatuluk::readFile(text) :
                                std::vector<char>(text.begin(), text.end());

// Create output vector.
std::vector<std::int32_t> output(input.size());

// Create scanner instance.
gimbatuluk::PFAC pfac(device, input.size());
std::cout << "Using Device: " << pfac.getDeviceName() << std::endl;

// Read entire dictionary file into memory.
pfac.loadDictionary(gimbatuluk::readFile(dictionary));

// Compile and install dictionary onto Device.
pfac.installDictionary();

// Do a synchronous scan to compute the expected result.
pfac.scan(input, output);

// Display results.
std::cout << std::endl;
for (auto c : output) {
    std::cout << c << " ";
}
std::cout << std::endl;
````

There are still an number of optimisations yet to be implemented, for example using page locked/pinned memory, and the OpenCL CPU Kernel is currently sub-optimal.

The code layout could do with a refactor, in particular the examples need to be placed in their own directory and the library should be made rather more self-contained.
