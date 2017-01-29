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
    std::string dictionary = "words";
    std::string _usage = 
        "Usage: " + std::string(argv[0]) + " [OPTIONS]\n" \
        "Options:\n" \
        "  -h, --help                     show this help message and exit\n" \
        "  -l, --list                     list available devices and exit\n" \
        "  -D <device>, --device <device> device to use\n" \
        "  -d <dict>, --dictionary <dict> dictionary file to use, default = " + dictionary + "\n" \
        "  -t <text>, --text <text>       text file to use, default = stdin\n";

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

        // Create output vector.
        std::vector<std::int32_t> output(input.size());

        // Create scanner instance. In this example it's important to create it
        // after the output vector as we don't want the otput vector to go out
        // of scope until after the callback has been called.
        gimbatuluk::PFAC pfac(device, input.size());

        // Read entire dictionary file into memory.
        pfac.loadDictionary(gimbatuluk::readFile(dictionary));
        std::cout << "Using Device: " << pfac.getDeviceName() << std::endl;

        // Compile and install dictionary onto Device.
        pfac.installDictionary();

        // Do a asynchronous scan to compute the expected result.
        pfac.scan(input, output, [](const std::vector<char>& input,
                                    std::vector<std::int32_t>& output) {
            // Display results.
            std::cout << std::endl;
            for (auto c : output) {
                std::cout << c << " ";
            }
            std::cout << std::endl;
        });

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::
             duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "\noverall time = " << duration << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error, caught exception: " << e.what() << std::endl;
    }
}

