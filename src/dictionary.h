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

#include <array>
#include <cstdint>
#include <vector>

namespace gimbatuluk {

constexpr std::int32_t INVALID = -1;
constexpr std::int32_t MASKBITS = 16;
constexpr std::int32_t MASK = 0x0000FFFF;

/**
 * 257 is the prime number used in the hash function and has the useful
 * property that we can do reduction modulo 257 using (x & 255) - (x >> 8)
 * http://mymathforum.com/number-theory/11914-calculate-10-7-mod-257-a.html
 */
constexpr int P257 = 257;
constexpr int mod257(int x) {
    int mod = (x & 255) - (x >> 8);
    if (mod < 0) {
        mod += P257;
    }
    return mod;
}


struct Transition {
    std::int32_t ch;
    std::int32_t nextState;
};

struct HashRow {
    std::int32_t offset;
    std::int32_t k_sminus1; // Packed using k << 16 | sminus1
};


struct Dictionary {
    Dictionary() = default;
    ~Dictionary() = default;

    // Explicitly make Dictionary non-copyable and non-movable
    Dictionary(Dictionary&&) = delete;
    Dictionary(const Dictionary&) = delete;
    Dictionary& operator=(Dictionary&&) = delete;
    Dictionary& operator=(const Dictionary&) = delete;

    void clear();
    void load(const std::vector<char>& buffer);
    void createHashTable();

    /**
     * Raw state table from which we create the hash tables. The table rows
     * represent states (nodes) and the columns represent transitions (arcs)
     */
    std::vector<std::vector<Transition>> stateTable;

    /**
     * Index of the initial state, N.B. this is set to be the number of patterns
     * in the dictionary *not* zero, this is so we can then cheaply identify
     * "match" states because their index will be < initialState. 
     */
    std::int32_t initialState;

    /**
     * Compiled state table information. The hashRow is indexed by state index
     * and contains an offset into the hashVal table plus the k and s - 1 hash
     * function values for the state. The hashVal contains the ch and nextState.
     * The first transition uses a simple array lookup rather than hashing.
     */
    std::array<std::int32_t, 256> initialTransitions;
    std::vector<HashRow> hashRow;
    std::vector<Transition> hashVal;
};

} // namespace gimbatuluk

