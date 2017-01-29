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

// Private implementation header, not part of public API

#pragma once

#include "circular-store.h"
#include "pfac.h"
#include "scanner.h"

#define __CL_ENABLE_EXCEPTIONS
#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace gimbatuluk {

template<std::size_t N>
struct CallbackWrapper {
    Callback callback = [](const std::vector<char>& input,
                           std::vector<std::int32_t>& output) {};

    const std::vector<char>* input;
    std::vector<std::int32_t>* output;
    CircularStore<CallbackWrapper<N>, N>* store;

    cl::Event bufferReadEvent;
};

class OpenCLScanner: public Scanner { // Made non-copyable & non-movable by Scanner
public:
    static std::vector<std::string> getAvailableDevices();

    OpenCLScanner(const std::string deviceName,
                  const std::size_t bufferSize,
                  const Dictionary& dictionary);

    std::string getDeviceName() override;
    void installDictionary() override;

    void scan(const std::vector<char>& input,
              std::vector<std::int32_t>& output) override;
    void scan(const std::vector<char>& input,
              std::vector<std::int32_t>& output, Callback callback) override;

    void scan(const std::vector<char>& input,
              std::vector<MatchEntry>& output,
              const std::int32_t limit) override;
private:
    /**
     * Number of OpenCL CommandQueues. For a synchronous scan we only need a
     * single CommandQueue but for the async scan we need multiple CommandQueues
     * and buffers so that overlapped data transfers can occur.
     */
    static constexpr auto COMMAND_QUEUES = 3;
    static constexpr auto BUFFERS = 3;

    void initialiseOpenCL();

    const std::string deviceName;
    const std::size_t bufferSize;
    const Dictionary* dictionary; // *Non-owned* association to Dictionary.
    int scanCount; // Count of async scan calls, used to identify CommandQueue.

    /**
     * OpenCL Objects used to initialise and run the OpenCL program. N.B. the
     * C++ classes are thin reference counting Wrappers round the OpenCL C types
     * as per cl.hpp, their copy and assignment functions perform a shallow copy
     * and/or manage reference counting of the underlying OpenCL Objects.
     * Note that we have multiple distinct CommandQueue instances so that we may
     * overlap the write, execute & read operations to optimise data transfers.
     */ 
    cl::Device device;
    cl::Context context;
    cl::Kernel pfacKernel;        // Kernel for running PFAC.
    cl::Kernel pfacCompactKernel; // Kernel for running PFAC followed by compaction.
    std::array<cl::CommandQueue, COMMAND_QUEUES> queue;

    // The callbackStore holds callback state wrapper objects for each CommandQueue.
    CircularStore<CallbackWrapper<COMMAND_QUEUES>, COMMAND_QUEUES> callbackStore;

    // Device I/O buffers. With multiple command queues double buffering is used.
    // sharedMemory is used to communicate betwen Work Groups in pfacCompactKernel.
    std::array<cl::Buffer, BUFFERS> inBuffer;
    std::array<cl::Buffer, BUFFERS> outBuffer;
    std::array<cl::Buffer, BUFFERS> sharedMemory;
    std::vector<int>                sharedMemoryInitialValue;

    // Hash table objects are mapped to GPU texture memory (because it's cached).
    cl::Image1DBuffer initialTransitions;
    cl::Image1DBuffer hashRow;
    cl::Image1DBuffer hashVal;
};

} // namespace gimbatuluk

