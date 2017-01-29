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

#include "pfac.h"
#include "scanner.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace gimbatuluk {

class CPUScanner: public Scanner { // Made non-copyable & non-movable by Scanner
public:
    static std::vector<std::string> getAvailableDevices();

    CPUScanner(const std::string deviceName,
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
    const std::string deviceName;
    const Dictionary* dictionary; // *Non-owned* association to Dictionary.
};

} // namespace gimbatuluk

