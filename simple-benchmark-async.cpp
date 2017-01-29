/**
 * Copyright 2016 Fraser Adams
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */

#include "pfac.h"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

/**
 * Simple benchmark for the asynchronous text scanner. Parses command line
 * arguments then reads the dictionary and input text and times the pattern
 * scanner over a number of iterations to determine the throughput.
 *
 * N.B. the callback for this example is a simple "do nothing" lambda, which
 * is clearly not representative of a real scenario. Using this pattern, with
 * the input and output data being reused on each iteration will result in
 * corrupted data for most real examples as data transfers are asynchronous
 * and there may be up to three (the number of command queues) transfers and
 * callbacks pending, so realistic asynchrononous implementations should use
 * multiple (at least three) input and output vectors/data blocks. The
 * soak-test-async example illustrates one approach using multiple data objects
 * each containing their own input/output vector, but any approach that avoids
 * reuse of the input/output vectors whilst the callback is pending should work.
 */
int main(int argc, char** argv) {
    int iterations = 10000;
    std::string dictionary = "words";
    std::string text = "the fat cat sat on the mat and acted like a prat";
    std::string _usage = 
        "Usage: " + std::string(argv[0]) + " [OPTIONS]\n" \
        "Options:\n" \
        "  -h, --help                     show this help message and exit\n" \
        "  -l, --list                     list available devices and exit\n" \
        "  -D <device>, --device <device> device to use\n" \
        "  -d <dict>, --dictionary <dict>   dictionary file to use, default = " + dictionary + "\n" \
        "  -t <text>, --text <text>       text file to use, default = stdin\n" \
        "  -s <size>, --size <size>         data size, default = text size\n" \
        "  -i <count>, --iterations <count> number of iterations, default = " + std::to_string(iterations) + "\n" \
        "Examples:\n" \
        "  # Scan \"" + text + "\"\n" \
        "  # padded out to 1300000 bytes for " + std::to_string(iterations) + " iterations\n" \
        "  " + std::string(argv[0]) + " -s 1300000\n\n" \
        "  # Scan the text of the file \"words\" for 1000 iterations\n" \
        "  " + std::string(argv[0]) + " -t words -i 1000\n\n" \
        "  # Scan the text of the file \"words\" for " + std::to_string(iterations) + " iterations\n" \
        "  # using the second OpenCL GPU Device (if available)\n" \
        "  " + std::string(argv[0]) + " -t words -D OpenCL:GPU[1]\n\n";

    std::string device = gimbatuluk::PFAC::getAvailableDevices()[0];
    bool textIsFile = false;
    int size = 0;

    if (argc > 1) {
        if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
            std::cout << _usage;
            std::exit(EXIT_SUCCESS);
        } else if (std::string(argv[1]) == "-l" || std::string(argv[1]) == "--list") {
            for (auto device : gimbatuluk::PFAC::getAvailableDevices()) {
                std::cout << device << std::endl;
            }
            std::exit(EXIT_SUCCESS);
        }

        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg[0] == '-') {
                i++;
                std::string val = argv[i];
                if (arg == "-D" || arg == "--device") {
                    device = val;
                } else if (arg == "-d" || arg == "--dictionary") {
                    dictionary = val;
                } else if (arg == "-t" || arg == "--text") {
                    text = val;
                    textIsFile = true;
                } else if (arg == "-s" || arg == "--size") {
                    size = std::stoi(val);
                } else if (arg == "-i" || arg == "--iterations") {
                    iterations = std::stoi(val);
                }
            } else {
                text = arg;
            }
        }
    }

    try {        
        // Read the text we want to scan into memory.
        auto input = textIsFile ? gimbatuluk::readFile(text) :
                                  std::vector<char>(text.begin(), text.end());

        if (size > 0) {
            input.resize(size);
        }

        const auto DATA_SIZE_MB = input.size()*1e-6*iterations;
        const auto DATA_SIZE_GB = input.size()*1e-9*iterations;

        // Create output vector.
        std::vector<std::int32_t> output(input.size());

        // Create scanner instance.
        gimbatuluk::PFAC pfac(device, input.size());
        std::cout << "Using Device: " << pfac.getDeviceName() << std::endl;

        // Read entire dictionary file into memory.
        pfac.loadDictionary(gimbatuluk::readFile(dictionary));

        // Compile and install dictionary onto Device.
        pfac.installDictionary();

        auto start = std::chrono::steady_clock::now();

        for (auto i = 0; i < iterations; i++) {
            pfac.scan(input, output, [](const std::vector<char>& input,
                      std::vector<std::int32_t>& output) {});
        }

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::
            duration_cast<std::chrono::milliseconds>(end - start).count()/1000.0;

        std::cout << "Data size = " << input.size() << std::endl;
        std::cout << "Iterations = " << iterations << std::endl;
        std::cout << "\nscan time = " << duration << std::endl;
        std::cout << "data sent (MB) = " << DATA_SIZE_MB << std::endl;
        std::cout << "data sent (GB) = " << DATA_SIZE_GB << std::endl;
        std::cout << "bandwidth (MB/s) = " << DATA_SIZE_MB/duration << std::endl;
        std::cout << "bandwidth (GB/s) = " << DATA_SIZE_GB/duration << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error, caught exception: " << e.what() << std::endl;
    }
}

