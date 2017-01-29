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

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace gimbatuluk {

class Scanner {
public:
    Scanner() = default;
    virtual ~Scanner() = default;

    // Explicitly make Scanner and subclasses non-copyable and non-movable
    Scanner(Scanner&&) = delete;
    Scanner(const Scanner&) = delete;
    Scanner& operator=(Scanner&&) = delete;
    Scanner& operator=(const Scanner&) = delete;

    virtual std::string getDeviceName() = 0;
    virtual void installDictionary() = 0;

    virtual void scan(const std::vector<char>& input,
                      std::vector<std::int32_t>& output) = 0;
    virtual void scan(const std::vector<char>& input,
                      std::vector<std::int32_t>& output, Callback callback) = 0;

    virtual void scan(const std::vector<char>& input,
                      std::vector<MatchEntry>& output,
                      const std::int32_t limit) = 0;
};

} // namespace gimbatuluk

