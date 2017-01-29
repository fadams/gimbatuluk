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
 * Simple text scanner. Parses command line arguments then reads the dictionary
 * and input text and runs the pattern scanner before returning the matched
 * results.
 */
int main(int argc, char** argv) {
    int iterations = 10000000;
    std::string dictionary = "words";
    std::string _usage = 
        "Usage: " + std::string(argv[0]) + " [OPTIONS]\n" \
        "Options:\n" \
        "  -h, --help                     show this help message and exit\n" \
        "  -l, --list                     list available devices and exit\n" \
        "  -D <device>, --device <device> device to use\n" \
        "  -d <dict>, --dictionary <dict> dictionary file to use, default = " + dictionary + "\n" \
        "  -t <text>, --text <text>       text file to use, default = stdin\n" \
        "  -i <count>, --iterations <count> number of iterations, default = " + std::to_string(iterations) + "\n";

    std::string device = gimbatuluk::PFAC::getAvailableDevices()[0];
    std::string text = "the fat cat sat on the mat and acted like a prat";
    bool textIsFile = false;

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

        // Create output vectors.
        std::vector<std::int32_t> output(input.size());
        std::vector<gimbatuluk::MatchEntry> compactOutput(input.size());

        // Create scanner instance.
        gimbatuluk::PFAC pfac(device, input.size());
        std::cout << "Using Device: " << pfac.getDeviceName() << std::endl;

        // Read entire dictionary file into memory.
        pfac.loadDictionary(gimbatuluk::readFile(dictionary));

        // Compile and install dictionary onto Device.
        pfac.installDictionary();

        for (auto i = 0; i < iterations; i++) {
            if (i % 100 == 0) {
                std::cout << "iteration " << i << std::endl;
            }
            auto in = input;

            for (auto j = 0u; j < in.size(); j++) {
                // Do a synchronous scan to compute the expected result.
                pfac.scan(in, output);

                long int count1 = 0;
                for (auto i = 0u; i < output.size(); i++) {
                    const auto c = output[i];
                    if (c != -1) {
                        count1 += i + c;
                    }
                }

                // Do a synchronous compact scan to compute the expected result.
                pfac.scan(in, compactOutput);

                long int count2 = 0;
                for (auto c : compactOutput) {
                    count2 += c.index + c.value;

                }

                if (count1 != count2) {
                    std::cout << "Failure: the scan and compact scan results are different" << std::endl;
                    std::abort();
                }

                // Set character to space so next run has different results.
                in[j] = ' ';
            }
        }

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::
             duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "\noverall time = " << duration << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error, caught exception: " << e.what() << std::endl;
    }
}

