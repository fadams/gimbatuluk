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

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace gimbatuluk {

/**
 * The "compact" scan returns an array where each element represents a match
 * and comprises the index within the input and the pattern ID of each match.
 */
struct MatchEntry {
    std::int32_t index;
    std::int32_t value;
};

using Callback = std::function<void(const std::vector<char>& input, 
                               std::vector<std::int32_t>& output)>;

std::vector<char> readFile(const std::string& fileName);

struct Dictionary;
class Scanner;
class PFAC {
public:
    static std::vector<std::string> getAvailableDevices();

    PFAC();
    PFAC(const std::size_t maxBufferSze);
    PFAC(const std::string deviceName);
    PFAC(const std::string deviceName, const std::size_t bufferSize);
    ~PFAC();

    PFAC(PFAC&&);
    PFAC(const PFAC&) = delete;
    PFAC& operator=(PFAC&&);
    PFAC& operator=(const PFAC&) = delete;

    std::string getDeviceName();
    void clearDictionary();
    void loadDictionary(const std::vector<char>& buffer);
    void installDictionary();

    // Scan producing output of pattern IDs at the index of the match location.
    void scan(const std::vector<char>& input,
              std::vector<std::int32_t>& output);
    void scan(const std::vector<char>& input,
              std::vector<std::int32_t>& output, Callback callback);

    // Scan producing compact output of index/value pairs, limit is the maximum
    // number of matches to be populated in order to constrain bandwidth.
    void scan(const std::vector<char>& input,
              std::vector<MatchEntry>& output,
              const std::int32_t limit = -1);
private:
    std::unique_ptr<Dictionary> dictionary;
    std::unique_ptr<Scanner> scanner;
};

} // namespace gimbatuluk


