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

#include "circular-store.h"
#include "pfac.h"
#include "dictionary.h"
#include "scanner.h"
#include "scanner-opencl.h"

/**
 * Include OpenCl using the C++ Wrapper API defined here:
 * http://www.khronos.org/registry/cl/specs/opencl-cplusplus-1.2.pdf
 * Note that the OpenCL Wrapper API is more idiomatically c++98 than c++11
 * so to get the available OpenCL Platforms one would do:
 *    std::vector<cl::Platform> platforms;
 *    cl::Platform::get(&platforms);
 * as opposed to say:
 *    std::vector<cl::Platform> platforms cl::Platform::get();
 *
 * it's still less of a chore than using the vanilla C API though and the
 * templated getInfo<property_name> approach is a fairly neat way to handle
 * the range of different return types available from getInfo(). The C++
 * wrapper also handles reference counting of the underlying OpenCL objects
 * so the lifecycle of the C++ wrapping objects is fairly natural.
 */
#define __CL_ENABLE_EXCEPTIONS
#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

/**
 * Monkey Patch Alert!!
 * CL_DEVICE_IMAGE_MAX_BUFFER_SIZE PARAM_TRAITS *should* be included in cl.hpp,
 * but it isn't! As it is possible that it may be added in some later release we
 * probe for its presence in the build system (CMakeLists.txt) and if it's not
 * present we define DECLARE_CL_DEVICE_IMAGE_MAX_BUFFER_SIZE_PARAM_TRAITS so
 * we can create our own PARAM_TRAITS below.
 */
#if defined(DECLARE_CL_DEVICE_IMAGE_MAX_BUFFER_SIZE_PARAM_TRAITS)
namespace cl {
    namespace detail {
        __CL_DECLARE_PARAM_TRAITS(cl_device_info,
                                  CL_DEVICE_IMAGE_MAX_BUFFER_SIZE,
                                  std::size_t)
    } // namespace detail
} // namespace cl
#endif

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace gimbatuluk {

constexpr auto PROGRAM_PATH = "./";
constexpr auto GPU_PROGRAM_NAME = "pfac-opencl-gpu.cl";
constexpr auto CPU_PROGRAM_NAME = "pfac-opencl-cpu.cl";

/**
 * See https://www.khronos.org/registry/cl/sdk/1.0/docs/man/xhtml/clBuildProgram.html
 * We probably don't need floating point optimisations so keep it simple for now.
 */
constexpr auto PROGRAM_BUILD_OPTIONS = "-Werror";

/**
 * N.B. The kernel expects WORK_GROUP_SIZE to equal the maximum number of
 * transitions for a state e.g. 256. If the WORK_GROUP_SIZE needs to be another
 * value then the kernel code to load initialTransitionsCache will need updating.
 * MAX_PATTERN_SIZE should be greater than the maximum pattern length
 * divided by sizeof(int), so 128 would support a maximum pattern length of 512.
 */
constexpr auto WORK_GROUP_SIZE = 256;
constexpr auto MAX_PATTERN_SIZE = 128;


/**
 * Enumerate all available OpenCL Devices across all available OpenCL Platforms.
 * Puts GPU Devices at the front and CPU Devices at the back irrespective of
 * Platform so we don't default to a CPU Devices just because the first Platform
 * happens to be a CPU only Platform. This free function returns a vector of
 * pairs containing the human readable device name and the cl::Device instance.
 * This function is a helper function and only used internally by OpenCLScanner.
 */
static std::vector<std::pair<std::string, cl::Device>> getAvailableDevices() {
    // Default device type is GPU if available.
    std::vector<std::pair<std::string, cl::Device>> devices;

    std::vector<std::pair<std::string, cl::Device>> accelerators;
    std::vector<std::pair<std::string, cl::Device>> cpus;

    std::vector<cl::Platform> clplatforms;
    cl::Platform::get(&clplatforms);

    for (auto platform : clplatforms) {
        try {
            std::vector<cl::Device> cldevices;
            platform.getDevices(CL_DEVICE_TYPE_ALL, &cldevices);
            for (auto device : cldevices) {
                const cl_device_type type = device.getInfo<CL_DEVICE_TYPE>();
                std::string name = device.getInfo<CL_DEVICE_NAME>();
                if (type == CL_DEVICE_TYPE_GPU) {
                    name = "OpenCL:GPU[" +
                            std::to_string(devices.size()) +
                            "]:" + name;
                    devices.emplace_back(name, device);
                } else if (type == CL_DEVICE_TYPE_ACCELERATOR) {
                    name = "OpenCL:ACCELERATOR[" +
                            std::to_string(accelerators.size()) +
                            "]:" + name;
                    accelerators.emplace_back(name, device);
                } else if (type == CL_DEVICE_TYPE_CPU) {
                    name = "OpenCL:CPU[" +
                            std::to_string(cpus.size()) +
                            "]:" + name;
                    cpus.emplace_back(name, device);
                }
            }
        } catch (...) {
            // Ignore any OpenCL Device exceptions, simply don't add the device.
        } 
    }

    // Add any accelerators and CPU devices after the GPU devices.
    devices.insert(devices.cend(), std::make_move_iterator(accelerators.cbegin()),
                                   std::make_move_iterator(accelerators.cend()));
    devices.insert(devices.cend(), std::make_move_iterator(cpus.cbegin()),
                                   std::make_move_iterator(cpus.cend()));

    return devices;
}

/**
 * Enumerate all available OpenCL Devices across all available OpenCL Platforms.
 * Puts GPU Devices at the front and CPU Devices at the back irrespective of
 * Platform so we don't default to a CPU Devices just because the first Platform
 * happens to be a CPU only Platform. This externally visible class method
 * only returns the unique human readable Device names which may subsequently
 * be used when constructing a PFAC instance to create an instance using a
 * particular device.
 */
std::vector<std::string> OpenCLScanner::getAvailableDevices() {
    std::vector<std::string> devices;
    for (auto devicesKV : gimbatuluk::getAvailableDevices()) {
        devices.emplace_back(devicesKV.first);
    }

    return devices;
}


OpenCLScanner::OpenCLScanner(const std::string deviceName,
                             const std::size_t bufferSize,
                             const Dictionary& dictionary):
deviceName(deviceName),
bufferSize(bufferSize),
dictionary(&dictionary),
scanCount(0) {
//    std::cout << "\tOpenCLScanner Constructor deviceName = " << deviceName << ", bufferSize " << bufferSize << std::endl;
//    std::cout << "\tthis = " << this << std::endl;
//    std::cout << "\tDictionary = " << this->dictionary << std::endl;
}

/**
 * Initialise the key parts of OpenCL required by the other methods. The first
 * thing to do is to find the OpenCL Device that corresponds to the specified
 * OpenCLScanner Device name. If that fails all bets are off so an exception is
 * thrown, if it succeeds the Device is used to create an OpenCL Context and
 * the OpenCL Program source is loaded the Program is built and the Kernel(s)
 * extracted. N.B. this is an expensive method as it is creating a Context and
 * loading and compiling the OpenCL Program, so it should only be called if
 * the OpenCL Device has not already been initialised.
 */
void OpenCLScanner::initialiseOpenCL() {
//    std::cout << "\t\tOpenCLScanner::initialiseOpenCL" << std::endl;

    /**
     * Find the OpenCL Device that corresponds to the specified Device name.
     * An incomplete deviceName may be specified and this search will return the
     * first match, e.g. OpenCL:GPU[0] may be supplied rather than the full name.
     */
    for (auto devicesKV : gimbatuluk::getAvailableDevices()) {
        if (devicesKV.first.find(deviceName) != std::string::npos) {
            device = devicesKV.second;
            break;
        }
    }

    /**
     * The function call operator on cl::Device returns the underlying OpenCL
     * Object, which can be used to determine if the Device is initialised.
     */
    if (device() == nullptr) {
        std::string message = "Failed to find Device \"" +  deviceName + "\"";
        throw std::runtime_error(message);
    }

//    const bool USE_MEMORY_MAPPING = device.getInfo<CL_DEVICE_HOST_UNIFIED_MEMORY>() > 0;

    // Create the OpenCL Context using the OpenCL Device that we've found.
    context = cl::Context(device);

    // Select the GPU or CPU optimised program. TODO GPUs other than Nvidia/AMD
    const auto PROGRAM_NAME = (device.getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_CPU) ? 
                                CPU_PROGRAM_NAME : GPU_PROGRAM_NAME;

    // TODO implement a rather more robust search path for OpenCL Program...
    const auto sourceBytes = gimbatuluk::readFile(std::string(PROGRAM_PATH) +
                                                  std::string(PROGRAM_NAME));

    /**
     * Create OpenCL Program from the sources (we only use one source but sources
     * is actually a vector of data/length pairs so could use many source files)
     */
    const cl::Program::Sources sources = {{sourceBytes.data(), sourceBytes.size()}};
    cl::Program program = cl::Program(context, sources);

    /**
     * Build the OpenCL Program, if an exception occurs we retrieve the log and
     * throw a more complete exception containing the compiler error message.
     */
    try {
        std::string options = PROGRAM_BUILD_OPTIONS;
        // Pass constants to OpenCL Program.
        options += " -DINVALID=" + std::to_string(INVALID);
        options += " -DMASKBITS=" + std::to_string(MASKBITS);
        options += " -DMASK=" + std::to_string(MASK);
        options += " -DWORK_GROUP_SIZE=" + std::to_string(WORK_GROUP_SIZE);
        options += " -DMAX_PATTERN_SIZE=" + std::to_string(MAX_PATTERN_SIZE);

        // Enable Warp/Wavefront optimisations. TODO do other vendors use this approach?
        const std::string vendor = device.getInfo<CL_DEVICE_VENDOR>();
        if (vendor.find("NVIDIA") != std::string::npos) {
            options += " -DWARP_SIZE=32";
            options += " -DWARP_SHIFT=5";
        } else if (vendor.find("Advanced Micro Devices") != std::string::npos) {
            options += " -DWARP_SIZE=64";
            options += " -DWARP_SHIFT=6";
        } else {
            options += " -DWARP_SIZE=1";
            options += " -DWARP_SHIFT=0";
        }

        program.build(options.data());
    } catch (const cl::Error e) {
        std::string message = "Failed to build OpenCL Program.\nBuild Log:\n" +         
                    program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
        throw std::runtime_error(message);
    }

//std::cout << "\n" << program.getInfo<CL_PROGRAM_BINARIES>()[0] << std::endl;

    // Extract the Kernels we're going to execute from the Program.
    pfacKernel = cl::Kernel(program, "pfac");
    pfacCompactKernel = cl::Kernel(program, "pfacCompact");

    /**
     * Create the OpenCL CommandQueues to which we push commands for the Device.
     * Note that we have multiple distinct CommandQueue instances so that we may
     * overlap the write, execute, read operations thus optimising data transfers.
     */
    for (auto i = 0u; i < COMMAND_QUEUES; i++) {
        queue[i] = cl::CommandQueue(context, device);
    }

    /**
     * Compute the number of Work Groups required to process bufferSize
     * which is necessary to calculate the maximum required size of sharedMemory
     * n = number of OpenCL integers that would completely contain bufferSize.
     */
    const cl_int n = (bufferSize + sizeof(cl_int) - 1)/sizeof(cl_int);
    const auto workGroups = (n + WORK_GROUP_SIZE - 1)/WORK_GROUP_SIZE;
    sharedMemoryInitialValue.resize(workGroups*2, INVALID); // Struct of two ints.

//std::cout << "bufferSize = " << bufferSize << std::endl;
//std::cout << "workGroups = " << workGroups << std::endl;

    // Pre-allocate device buffers.
    for (auto i = 0u; i < BUFFERS; i++) {
        // inBuffer is a char sequence.
        inBuffer[i] = cl::Buffer(context, CL_MEM_READ_ONLY,
                                 bufferSize);
        /**
         * outBuffer is an int sequence. It has a size of bufferSize*2 because
         * pfacCompactKernel returns an array of pairs of ints representing
         * the position and pattern ID of matches. In practice as the point of
         * using the pfacCompactKernel is to reduce output bandwidth if the
         * number of matching results exceeds ~bufferSize/2 then the system
         * performance will actually be worse than the plain pfacKernel.
         * TODO if Device Buffer size is an issue providing an optional global 
         * limit on number of results returned by pfacCompactKernel may be an option.
         */  
        outBuffer[i] = cl::Buffer(context, CL_MEM_WRITE_ONLY,
                                  bufferSize*2*sizeof(cl_int));
        /**
         * sharedMemory is an int sequence. It is used in the pfacCompactKernel
         * as a mechanism for synchronising/communicating between Work Groups.
         * It comprises a struct of two ints: workGroupSum and inclusivePrefix.
         */
        sharedMemory[i] = cl::Buffer(context, CL_MEM_READ_WRITE,
                                     workGroups*2*sizeof(cl_int));
    }
}

std::string OpenCLScanner::getDeviceName() {
    return deviceName;
}

void OpenCLScanner::installDictionary() {
//    std::cout << "\t\tOpenCLScanner::installDictionary" << std::endl;

    /**
     * The function call operator on cl::Device returns the underlying OpenCL
     * Object, which can be used to determine if the Device is initialised.
     */
    if (device() == nullptr) {
        initialiseOpenCL();
    }

    /**
     * Query maximum number of pixels for a 1D image created from a buffer object. 
     * For CUDA devices with compute capability > 2.x this should be 2^27 i.e.
     * 134217728 (pixels not bytes), but OpenCL specifies a minimum of 65536
     * so we query the value rather than just use 1 << 27.
     */
    const std::size_t IMAGE1D_MAX_BUFFER_SIZE = 
        device.getInfo<CL_DEVICE_IMAGE_MAX_BUFFER_SIZE>();
//std::cout << "CL_DEVICE_IMAGE_MAX_BUFFER_SIZE" << std::endl;
//std::cout << IMAGE1D_MAX_BUFFER_SIZE << std::endl;

    /**
     * TODO rather than throw an exception here it is probably better to fall
     * back to using (uncached) global memory rather than using Image1DBuffer.
     */
    if (dictionary->hashVal.size() > IMAGE1D_MAX_BUFFER_SIZE) {
        std::string message = "Compiled dictionary size: " +
                               std::to_string(dictionary->hashVal.size()) +
                              " exceeds CL_DEVICE_IMAGE_MAX_BUFFER_SIZE: " +         
                               std::to_string(IMAGE1D_MAX_BUFFER_SIZE);
        throw std::runtime_error(message);
    }

    /**
     * Create initialTransitions, hashRow and hashVal tables on the OpenCL Device.
     * We use OpenCL 1.2 Image1DBuffers for the look-up tables this is because
     * texture memory is cached whereas global memory is not, so is likely slower.
     * Create an Image1DBuffer using the format of CL_RG, CL_SIGNED_INT32 which
     * is the equivalent of an int2 using the R and G pixel channels or for the
     * initialTransitions buffer using the format of CL_R, CL_SIGNED_INT32 which
     * is the equivalent of an int in the R pixel channel.
     *
     * Note that we create cl::Buffer supplying host_ptr and CL_MEM_USE_HOST_PTR
     * which loads the data directly onto the device, so we don't need to call
     * enqueueWriteImage for these.
     *
     * cl::Buffer wraps clCreateBuffer and expects a (non-const) void* host_ptr
     * for its 4th argument, so we have to const_cast hashVal.data()
     */

    // Host side hashRow, hashVal and initialTransitions
    const auto& hashRowH = dictionary->hashRow;
    const auto& hashValH = dictionary->hashVal;
    const auto& initialTransitionsH = dictionary->initialTransitions;

/*
    // TODO remove later.
    // Display the first few hashRow/hashVal values to check Image1DBuffer works.
    std::cout << "hashRowH" << std::endl;
    for (int i = 0; i < 10; i++){
        std::cout << hashRowH[i].offset << " " << hashRowH[i].k_sminus1 << " ";
    }
    std::cout << std::endl;
    std::cout << "hashValH" << std::endl;
    for (int i = 0; i < 10; i++){
        std::cout << hashValH[i].ch << " " << hashValH[i].nextState << " ";
    }
    std::cout << std::endl;
    // TODO remove later.
*/

    // N.B. The cl::Buffer sizes are bytes and cl::Image1DBuffer sizes are pixels

    cl::Buffer initialTransitionsBuffer(
        context,
        CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
        sizeof(std::int32_t)*initialTransitionsH.size(),
        const_cast<std::int32_t*>(initialTransitionsH.data())
    );

    initialTransitions = cl::Image1DBuffer(
        context,
        CL_MEM_READ_ONLY,
        cl::ImageFormat(CL_R, CL_SIGNED_INT32),
        initialTransitionsH.size(),
        initialTransitionsBuffer
    );

    cl::Buffer hashRowBuffer(
        context,
        CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
        sizeof(HashRow)*hashRowH.size(),
        const_cast<HashRow*>(hashRowH.data())
    );

    hashRow = cl::Image1DBuffer(
        context,
        CL_MEM_READ_ONLY,
        cl::ImageFormat(CL_RG, CL_SIGNED_INT32),
        hashRowH.size(),
        hashRowBuffer
    );

    cl::Buffer hashValBuffer(
        context,
        CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
        sizeof(Transition)*hashValH.size(),
        const_cast<Transition*>(hashValH.data())
    );

    hashVal = cl::Image1DBuffer(
        context,
        CL_MEM_READ_ONLY,
        cl::ImageFormat(CL_RG, CL_SIGNED_INT32),
        hashValH.size(),
        hashValBuffer
    );

/*
// These callbacks are temporary so I know that things are being deleted when I think.
initialTransitionsBuffer.setDestructorCallback([](cl_mem X, void *userData) {std::cout << "initialTransitionsBuffer destroyed\n";});
initialTransitions.setDestructorCallback([](cl_mem X, void *userData) {std::cout << "initialTransitions destroyed\n";});

hashRowBuffer.setDestructorCallback([](cl_mem X, void *userData) {std::cout << "hashRowBuffer destroyed\n";});
hashRow.setDestructorCallback([](cl_mem X, void *userData) {std::cout << "hashRow destroyed\n";});

hashValBuffer.setDestructorCallback([](cl_mem X, void *userData) {std::cout << "hashValBuffer destroyed\n";});
hashVal.setDestructorCallback([](cl_mem X, void *userData) {std::cout << "hashVal destroyed\n";});
*/
}


// Needs initialState, initialTransitions, hashRow, hashVal
void OpenCLScanner::scan(const std::vector<char>& input,
                         std::vector<std::int32_t>& output) {
    /**
     * The function call operator on cl::Kernel returns the underlying OpenCL
     * Object, which can be used to determine if the Kernel is initialised.
     */
    if (pfacKernel() == nullptr) {
        throw std::runtime_error("OpenCL pfacKernel uninitialised.");
    }

    // TODO scan size currently limited to cl_int (~2GB) - could support larger.
    const cl_int size = input.size();
    if (size == 0) {
        throw std::runtime_error("Input vector uninitialised.");
    }

    if (static_cast<std::size_t>(size) > bufferSize) {
        throw std::runtime_error("Input vector is larger than Device buffer.");
    }

    queue[0].enqueueWriteBuffer(inBuffer[0], CL_TRUE, 0, size, input.data());

    /**
     * The kernel processes the input characters in groups of four (OpenCL int),
     * so we therefore need to calculate our global work size in terms of how
     * many OpenCL integers it comprises. The computed global work size is then
     * rounded up to a multiple of the local work-group size (thread block size).
     */

    // Number of OpenCL integers that would completely contain the input bytes.
    const cl_int n = (size + sizeof(cl_int) - 1)/sizeof(cl_int);

    // Given n round up if necessary to a multiple of WORK_GROUP_SIZE.
    const auto r = n % WORK_GROUP_SIZE;
    const auto global = (r == 0) ? n : n + WORK_GROUP_SIZE - r;

//std::cout << "size = " << size << std::endl;
//std::cout << "n = " << n << std::endl;
//std::cout << "r = " << r << std::endl;
//std::cout << "global = " << global << std::endl;

    const cl_int initialState = dictionary->initialState;

    pfacKernel.setArg(0, initialTransitions);
    pfacKernel.setArg(1, hashRow);
    pfacKernel.setArg(2, hashVal);
    pfacKernel.setArg(3, initialState);
    pfacKernel.setArg(4, inBuffer[0]);
    pfacKernel.setArg(5, outBuffer[0]);
    pfacKernel.setArg(6, size);
    pfacKernel.setArg(7, n);

    queue[0].enqueueNDRangeKernel(pfacKernel,
                                  cl::NullRange, // Offset value is zero.
                                  cl::NDRange(global),
                                  cl::NDRange(WORK_GROUP_SIZE));

    output.resize(size);
    queue[0].enqueueReadBuffer(outBuffer[0], CL_TRUE, 0,
                               size*sizeof(cl_int), output.data());
}


// Async scan
void OpenCLScanner::scan(const std::vector<char>& input,
                         std::vector<std::int32_t>& output, Callback f) {
    /**
     * Use multiple command queues and synchronisation events so that we can
     * overlap the data transfers and kernel execution, which should allow
     * higher overall throughput than the synchronous scan.
     */

    /**
     * The function call operator on cl::Kernel returns the underlying OpenCL
     * Object, which can be used to determine if the Kernel is initialised.
     */
    if (pfacKernel() == nullptr) {
        throw std::runtime_error("OpenCL pfacKernel uninitialised.");
    }

    // TODO scan size currently limited to cl_int (~2GB) - could support larger.
    const cl_int size = input.size();
    if (size == 0) {
        throw std::runtime_error("Input vector uninitialised.");
    }

    if (static_cast<std::size_t>(size) > bufferSize) {
        throw std::runtime_error("Input vector is larger than Device buffer.");
    }

    auto& callback = callbackStore.get();
    callback.callback = std::move(f);
    callback.input = &input;
    callback.output = &output;
    callback.store = &callbackStore;

    auto qid = scanCount % COMMAND_QUEUES; // CommandQueue ID
    auto bid = scanCount % BUFFERS;        // Buffer ID
    scanCount++;

    queue[qid].enqueueWriteBuffer(inBuffer[bid], CL_FALSE, 0, size, input.data());

    /**
     * The kernel processes the input characters in groups of four (OpenCL int),
     * so we therefore need to calculate our global work size in terms of how
     * many OpenCL integers it comprises. The computed global work size is then
     * rounded up to a multiple of the local work-group size (thread block size).
     */

    // Number of OpenCL integers that would completely contain the input bytes.
    const cl_int n = (size + sizeof(cl_int) - 1)/sizeof(cl_int);

    // Given n round up if necessary to a multiple of WORK_GROUP_SIZE.
    const auto r = n % WORK_GROUP_SIZE;
    const auto global = (r == 0) ? n : n + WORK_GROUP_SIZE - r;

//std::cout << "size = " << size << std::endl;
//std::cout << "n = " << n << std::endl;
//std::cout << "r = " << r << std::endl;
//std::cout << "global = " << global << std::endl;

    const cl_int initialState = dictionary->initialState;

    pfacKernel.setArg(0, initialTransitions);
    pfacKernel.setArg(1, hashRow);
    pfacKernel.setArg(2, hashVal);
    pfacKernel.setArg(3, initialState);
    pfacKernel.setArg(4, inBuffer[bid]);
    pfacKernel.setArg(5, outBuffer[bid]);
    pfacKernel.setArg(6, size);
    pfacKernel.setArg(7, n);

    queue[qid].enqueueNDRangeKernel(pfacKernel,
                                    cl::NullRange, // Offset value is zero.
                                    cl::NDRange(global),
                                    cl::NDRange(WORK_GROUP_SIZE));

    output.resize(size);
    queue[qid].enqueueReadBuffer(outBuffer[bid], CL_FALSE, 0,
                                 size*sizeof(cl_int), output.data(),
                                 NULL, &callback.bufferReadEvent);

    callback.bufferReadEvent.setCallback(CL_COMPLETE,
                                [](cl_event event, cl_int status, void* c) {
        auto& callback = *static_cast<CallbackWrapper<COMMAND_QUEUES>*>(c);
        callback.callback(*callback.input, *callback.output);
        callback.store->release(callback);
    }, static_cast<void*>(&callback));
}


void OpenCLScanner::scan(const std::vector<char>& input,
                         std::vector<MatchEntry>& output,
                         const std::int32_t limit) {
    /**
     * The function call operator on cl::Kernel returns the underlying OpenCL
     * Object, which can be used to determine if the Kernel is initialised.
     */
    if (pfacCompactKernel() == nullptr) {
        throw std::runtime_error("OpenCL pfacCompactKernel uninitialised.");
    }

    // TODO scan size currently limited to cl_int (~2GB) - could support larger.
    const cl_int size = input.size();
    if (size == 0) {
        throw std::runtime_error("Input vector uninitialised.");
    }

    if (static_cast<std::size_t>(size) > bufferSize) {
        throw std::runtime_error("Input vector is larger than Device buffer.");
    }

    queue[0].enqueueWriteBuffer(inBuffer[0], CL_TRUE, 0, size, input.data());

    /**
     * The kernel processes the input characters in groups of four (OpenCL int),
     * so we therefore need to calculate our global work size in terms of how
     * many OpenCL integers it comprises. The computed global work size is then
     * rounded up to a multiple of the local work-group size (thread block size).
     */

    // Number of OpenCL integers that would completely contain the input bytes.
    const cl_int n = (size + sizeof(cl_int) - 1)/sizeof(cl_int);

    // Given n round up if necessary to a multiple of WORK_GROUP_SIZE.
    const auto r = n % WORK_GROUP_SIZE;
    const auto global = (r == 0) ? n : n + WORK_GROUP_SIZE - r;

    // We need the number of Work Groups in order to identify which sharedMemory
    // item contains the total number of matches.
    const auto workGroups = (n + WORK_GROUP_SIZE - 1)/WORK_GROUP_SIZE;

//std::cout << "size = " << size << std::endl;
//std::cout << "n = " << n << std::endl;
//std::cout << "r = " << r << std::endl;
//std::cout << "global = " << global << std::endl;
//std::cout << "workGroups = " << workGroups << std::endl;


    // Initialise the Work Group shared memory buffer. TODO Question, for
    // large numbers of Work Groups is it more efficient to launch a simple
    // Kernel that initialises the sharedMemory initial semaphore value?
    // TODO perhaps create a Device buffer holding the initial state then do
    // copy buffer to copy to sharedMemory is the fastest approach?
    queue[0].enqueueWriteBuffer(sharedMemory[0], CL_TRUE, 0,
                                workGroups*2*sizeof(cl_int), 
                                sharedMemoryInitialValue.data());




    const cl_int initialState = dictionary->initialState;

    const cl_int maxResults = (limit < 0 || limit > size) ? size : limit;
//std::cout << "maxResults = " << maxResults << std::endl;

    pfacCompactKernel.setArg(0, initialTransitions);
    pfacCompactKernel.setArg(1, hashRow);
    pfacCompactKernel.setArg(2, hashVal);
    pfacCompactKernel.setArg(3, initialState);
    pfacCompactKernel.setArg(4, inBuffer[0]);
    pfacCompactKernel.setArg(5, outBuffer[0]);
    pfacCompactKernel.setArg(6, sharedMemory[0]);
    pfacCompactKernel.setArg(7, size);
    pfacCompactKernel.setArg(8, n);
    pfacCompactKernel.setArg(9, maxResults);

    queue[0].enqueueNDRangeKernel(pfacCompactKernel,
                                  cl::NullRange, // Offset value is zero.
                                  cl::NDRange(global),
                                  cl::NDRange(WORK_GROUP_SIZE));

    /**
     * Retrieve the total number of matched values. This value is computed as
     * part of the Kernel's global prefix sum and the value contained in the
     * last Work Group contains the total number of matches. In the Kernel this
     * data is maintained in a global memory block shared across the Work Groups
     * and is held as a struct containing the workGroupSum (the total matches)
     * for the given Work Group and the inclusivePrefix which is the inclusive
     * scan result, that is the sum of workGroupSum for all the preceeding
     * Work Groups plus the workGroupSum for the current Work Group. This means
     * that the inclusivePrefix value for the last Work Group holds the total
     * number of matched values.
     */
    cl_int outputSize;
    queue[0].enqueueReadBuffer(sharedMemory[0], CL_TRUE,
                               ((workGroups - 1)*2 + 1)*sizeof(cl_int),
                               sizeof(cl_int), &outputSize);

    outputSize = maxResults < outputSize ? maxResults : outputSize;
//std::cout << "outputSize = " << outputSize << std::endl;

    output.resize(outputSize);
    queue[0].enqueueReadBuffer(outBuffer[0], CL_TRUE, 0,
                               outputSize*sizeof(MatchEntry), output.data());
}

} // namespace gimbatuluk

