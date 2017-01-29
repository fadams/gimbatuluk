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
 * AsyncData objects are callable and are primarily used as objects to pass
 * as the callback parameter for the async scan. AsyncData also holds the input
 * and output data vectors, because it is important not to reuse the input or
 * output data used in the async scan prematurely. The data is transfered to the
 * device asynchronously, so those vectors cannot be considered consistent (and
 * thus reusable) until the callback completes. There are other ways to achieve
 * this, and in practice only a maximum of three callbacks will be pending, so
 * one could use arrays of three input and output vectors and an index modulo
 * three (i % 3) to avoid premature reuse, but for a simple test like this we
 * just give each instance its own input and output vectors.
 */
struct AsyncData {
    std::vector<char> input;
    std::vector<std::int32_t> output;
    long int check; // Simple checksum to compare actual result with expected.

    AsyncData() = default;
    AsyncData(const AsyncData&) = delete; // disable copying

    void operator()(const std::vector<char>& input, std::vector<std::int32_t>& output) {
        // Check results against the stored checksum.
        long int count = 0;
        for (auto c : output) {
            count += c;
        }
//std::cout << "count " << count << " check " << check << std::endl;
        if (count != check) {
            std::cout << (long)this << " Failed, count " << count
                      << " should be " << check << std::endl;
            std::abort();
        }
    }
};

/**
 * Performs a soak test of the asynchronous scan. This is primarily to ensure
 * that we get correct results from the async scan over a large number of
 * iterations, as the mechanics of the async scan are a little bit fiddly due
 * to the asynchronous data transfers and can result in data corruption if the
 * output data vector is accessed early or if the input data vector is reused
 * prematurely. This test creates simple permutations of the input data and
 * cycles through the permutations a large number of times comparing the scan
 * result with the expected result, aborting if they don't match.
 */
int main(int argc, char** argv) {
    int iterations = 10000000;
    std::string dictionary = "words";
    std::string _usage = 
        "Usage: " + std::string(argv[0]) + " [OPTIONS]\n" \
        "Options:\n" \
        "  -h, --help                       show this help message and exit\n" \
        "  -l, --list                       list available devices and exit\n" \
        "  -D <device>, --device <device>   device to use\n" \
        "  -d <dict>, --dictionary <dict>   dictionary file to use, default = " + dictionary + "\n" \
        "  -t <text>, --text <text>         text file to use, default = stdin\n" \
        "  -i <count>, --iterations <count> number of iterations, default = " + std::to_string(iterations) + "\n";

    std::string device = gimbatuluk::PFAC::getAvailableDevices()[0];
    std::string text = "the fat cat sat on the mat and acted like a prat";
    bool textIsFile = false;

    // Limit the number of permutations, because if we don't do this a large
    // input file could cause memory usage to explode.
    constexpr int MAX_PERMUTATIONS = 100;

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
                } else if (arg == "-i" || arg == "--iterations") {
                    iterations = std::stoi(val);
                }
            } else {
                text = arg;
            }
        }
    }

    try {
        auto start = std::chrono::steady_clock::now();

        // Read the text we want to scan into memory.
        const auto input = textIsFile ? gimbatuluk::readFile(text) :
                                        std::vector<char>(text.begin(), text.end());

        // The test first does a synchronous scan to compute expected results.

        // Create output vector for the synchronous scan.
        std::vector<std::int32_t> output(input.size());

        // Create vector of AsyncData objects which are callable objects that
        // also hold the input and output data vectors.
        std::vector<AsyncData> data(input.size() > MAX_PERMUTATIONS ?       
                                    MAX_PERMUTATIONS : input.size());

        // Create scanner instance. In this example it's important to create it
        // after the AsyncData vector as we don't want the otput vector to go
        // out of scope until the callback has been called.
        gimbatuluk::PFAC pfac(device, input.size());
        std::cout << "Using Device: " << pfac.getDeviceName() << std::endl;

        // Read entire dictionary file into memory.
        pfac.loadDictionary(gimbatuluk::readFile(dictionary));

        // Compile and install dictionary onto Device.
        pfac.installDictionary();

        // Do the synchronous scan to compute the expected result.
        pfac.scan(input, output);

        // Compute sum of output values to compare input with expected result.
        long int count = 0;
        for (auto c : output) {
            count += c;
        }

        std::cout << "Creating test data" << std::endl;

        // Create array of test data and expected results by inserting spaces
        // (ascii 32) into input and modifying the expected result.
        data[0].input = input;
        data[0].output.resize(input.size());
        data[0].check = count; // Store simple checksum.

        for (auto i = 1u; i < data.size(); i++) {
            data[i].input = data[i - 1].input;
            data[i].input[i - 1] = ' '; // Add a space to the input vector.
            data[i].output.resize(input.size());
            count = (count - output[i - 1]) - 1; // Compute new checksum.
            data[i].check = count;
        }

        std::cout << "Starting test" << std::endl;

        // Run async scan for a large number of iterations comparing the actual
        // result with the expected result to validate correct behaviour.
        for (int i = 0; i < iterations; i++) {
            if (i % 100 == 0) {
                std::cout << "iteration " << i << std::endl;
            }
            auto& in = data[i % data.size()];
            pfac.scan(in.input, in.output, std::ref(in));
        }

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::
             duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Iterations = " << iterations << std::endl;
        std::cout << "\noverall time = " << duration << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error, caught exception: " << e.what() << std::endl;
    }
}

