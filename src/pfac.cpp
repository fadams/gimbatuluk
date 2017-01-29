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
#include "scanner-opencl.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace gimbatuluk {

// Polyfill make_unique etc. aliases to std::make_unique etc. if using >= c++14.
#include "c++14-polyfill.h"

constexpr std::size_t DEFAULT_BUFFER_SIZE = 150000000; // 150 MB

//------------------------------------------------------------------------------
// static free function prototype declarations.
static std::unique_ptr<Scanner> makeScanner(const std::string deviceName,
                                            const std::size_t bufferSize,
                                            const Dictionary& dictionary);

//------------------------------------------------------------------------------

/**
 * Utility free function to read a binary file into a std::vector<char> buffer.
 */
std::vector<char> readFile(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary | std::ios::ate);
    if (file) {
        std::streamsize size = file.tellg(); // Get file size.
        file.seekg(0, std::ios::beg); // Move file cursor back to start.

        std::vector<char> buffer(size);
        if (file.read(buffer.data(), size)) {
            return buffer;
        } else {
            std::string message = "Failed to read file \"" + 
                                   fileName + "\": " + std::strerror(errno);
            throw std::runtime_error(message);
        }
    } else {
        std::string message = "Failed to open file \"" +
                               fileName + "\": " + std::strerror(errno);
        throw std::runtime_error(message);
    }
}


//------------------------------------ PFAC ------------------------------------

/**
 * Enumerate all available devices attempting to put the highest performing
 * devices at the front of the list. The name of each device is in the form:
 * <OpenCL>|<Host>:<GPU>|<CPU>|<ACCELERATOR>[<number>]:<name> e.g.
 * OpenCL:GPU[0]:GeForce GTX TITAN X
 * The returned values may be used as constructor parameters, though the name
 * part is intended for human consumption and OpenCL:GPU[0] would be enough
 * if users know a-priori which devices are available.
 */
std::vector<std::string> PFAC::getAvailableDevices() {
    std::vector<std::string> devices = OpenCLScanner::getAvailableDevices();
    std::vector<std::string> cpuDevices = CPUScanner::getAvailableDevices();
    devices.insert(devices.cend(), std::make_move_iterator(cpuDevices.cbegin()),
                                   std::make_move_iterator(cpuDevices.cend()));
    return devices;
}


static std::unique_ptr<Scanner> makeScanner(const std::string deviceName,
                                            const std::size_t bufferSize,
                                            const Dictionary& dictionary) {
    if (deviceName.find("OpenCL") == 0) {
        return make_unique<OpenCLScanner>(deviceName, bufferSize, dictionary);
    } else if (deviceName.find("Host:CPU[0]") == 0) {
        return make_unique<CPUScanner>(deviceName, bufferSize, dictionary);
    } else {
        std::string message = "Failed to find Device \"" +  deviceName + "\"";
        throw std::runtime_error(message);
    }
}

PFAC::PFAC(): PFAC(getAvailableDevices()[0], DEFAULT_BUFFER_SIZE) {}
PFAC::PFAC(const std::string deviceName): PFAC(deviceName, DEFAULT_BUFFER_SIZE) {}

PFAC::PFAC(const std::string deviceName, const std::size_t bufferSize):
dictionary(make_unique<Dictionary>()),
scanner(makeScanner(deviceName, bufferSize, *dictionary)) {}

PFAC::~PFAC() = default;

// Use default move and assignment special member operations, they perform a move
// on the std::unique_ptr for dictionary and scanner, which is what we want.
PFAC::PFAC(PFAC&&) = default;
PFAC& PFAC::operator=(PFAC&&) = default;

std::string PFAC::getDeviceName() {
    return scanner->getDeviceName();
}

void PFAC::clearDictionary() {
    dictionary->clear();
}

void PFAC::loadDictionary(const std::vector<char>& buffer) {
    dictionary->load(buffer);
}

void PFAC::installDictionary() {
    dictionary->createHashTable();
    scanner->installDictionary();
}

void PFAC::scan(const std::vector<char>& input,
                std::vector<std::int32_t>& output) {
    scanner->scan(input, output);
}

void PFAC::scan(const std::vector<char>& input,
                std::vector<std::int32_t>& output, Callback callback) {
    scanner->scan(input, output, callback);
}

void PFAC::scan(const std::vector<char>& input,
                std::vector<MatchEntry>& output,
                const std::int32_t limit) {
    return scanner->scan(input, output, limit);
}

} // namespace gimbatuluk

