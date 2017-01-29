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
#include "dictionary.h"
#include "scanner.h"
#include "scanner-cpu.h"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// TODO CPU scan not yet implemented.

namespace gimbatuluk {

std::vector<std::string> CPUScanner::getAvailableDevices() {
    std::vector<std::string> devices;
    // devices.emplace_back("Host:CPU[0]:"); // TODO add this when implemented.
    return devices;
}


CPUScanner::CPUScanner(const std::string deviceName,
                       const std::size_t bufferSize,
                       const Dictionary& dictionary):
deviceName(deviceName),
dictionary(&dictionary) {
    std::cout << "\tCPUScanner Constructor deviceName = " << deviceName << ", bufferSize " << bufferSize << std::endl;
    std::cout << "\t\tNot yet implemented" << std::endl;
//    std::cout << "\tthis = " << this << std::endl;
//    std::cout << "\tDictionary = " << this->dictionary << std::endl;
}

std::string CPUScanner::getDeviceName() {
    return deviceName;
}

void CPUScanner::installDictionary() {
    std::cout << "\t\tCPUScanner::installDictionary" << std::endl;
    std::cout << "\t\tNot yet implemented" << std::endl;
}

// Needs initialState, initialTransitions, hashRow, hashVal
void CPUScanner::scan(const std::vector<char>& input,
                      std::vector<std::int32_t>& output) {
//    std::cout << "\t\tCPUScanner::scan, input.size() = " << input.size() << std::endl;
/*
    for (auto i = 0u; i < buffer.size(); i++) {
//console.log("i = " + i);
        std::uint32_t pos = i;
        std::uint8_t inputChar = buffer[pos];
//console.log("inputChar = " + String.fromCharCode(inputChar) + " (" + inputChar + ")");

        std::int32_t nextState = initialTransitions[inputChar];
        if (nextState != INVALID) {
            if (nextState < initialState){
                //match = nextState;
std::cout << "xx matched pattern " << nextState << " at " << i << std::endl;
            }
            pos = pos + 1;
            while (pos < buffer.size()) {
                inputChar = buffer[pos];

                // lookup next state
                const int2& row = hashRow[nextState];
                const std::int32_t offset = row.x;
                nextState = INVALID;
                if (offset >= 0) { 
                    const std::int32_t k_sminus1 = row.y;
                    const std::int32_t sminus1 = k_sminus1 & MASK;
                    const std::int32_t k = k_sminus1 >> MASKBITS; 

                    const std::int32_t p = mod257(k * inputChar) & sminus1;
                    const int2& value = hashVal[offset + p];
                    if (inputChar == value.x) {
                        nextState = value.y;
                    }
                }

                if (nextState == INVALID) {
                    break;
                }

                if (nextState < initialState) {
                    //match = nextState;
std::cout << "matched pattern " << nextState << " at " << i << std::endl;
                }
                pos = pos + 1;
            }
        }
    }
*/
}

void CPUScanner::scan(const std::vector<char>& input,
                      std::vector<std::int32_t>& output, Callback callback) {
//    std::cout << "\t\tCPUScanner::scanAsync" << std::endl;
//    std::cout << "\t\tNot yet implemented" << std::endl;
}

void CPUScanner::scan(const std::vector<char>& input,
                      std::vector<MatchEntry>& output,
                      const std::int32_t limit) {
//    std::cout << "\t\tCPUScanner::scan compact" << std::endl;
//    std::cout << "\t\tNot yet implemented" << std::endl;
}

} // namespace gimbatuluk

