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

#include <array>
#include <chrono>
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

//------------------------------------------------------------------------------
// static free function prototype declarations.
static std::int32_t getNextState(const std::vector<Transition>& currentState,
                                 const std::int32_t ch);

//--------------------------------- Dictionary ---------------------------------

/**
 * Helper function to return next state given the current state and input value.
 * TODO can't decide whether this should be replace by a lambda
 */
static std::int32_t getNextState(const std::vector<Transition>& currentState,
                                 const std::int32_t ch) {
    for (auto transition : currentState) {
        if (ch == transition.ch) {
            return transition.nextState;	
        }	
    }

    return INVALID;
}

void Dictionary::clear() {
    stateTable.clear();
}

void Dictionary::load(const std::vector<char>& buffer) {
//    std::cout << "Dictionary::loadDictionary, buffer.size() = " << buffer.size() << std::endl;

auto start = std::chrono::steady_clock::now();

    // Initial parse of dictionary file so that we can store the states that
    // represent matched patterns at the start of the state transition table.
    // By doing this we know that match states have an ID < the initialState ID
    for (auto i = 0u; i < buffer.size(); i++) {
        // If we reach the end of a non-empty line add a state to the table.
        if (buffer[i] == 10 && (i > 0 && buffer[i - 1] != 10)) {
            stateTable.push_back(std::vector<Transition>());
        }
    }

//    std::cout << "Number of patterns = " << stateTable.size() << std::endl;

    std::int32_t patternID = 0;
    initialState = stateTable.size();
    std::int32_t state = initialState;

    // Add a state representing the initial state to the table.
    stateTable.push_back(std::vector<Transition>());

    for (auto i = 0u; i < buffer.size(); i++) {
        const std::uint8_t ch = buffer[i];
        const bool isLastCharInPattern = i + 1 == buffer.size() || buffer[i + 1] == 10;

        if (ch == 10) { // Skip
        } else if (isLastCharInPattern) {
            // This state represents final (match) state of dictionary pattern.
            stateTable[state].push_back({ch, patternID});
//console.log("A: State %d {ch: %d, nextState: %d}", state, ch, patternID);
            state = initialState;
            patternID++;
        } else {
            std::int32_t nextState = getNextState(stateTable[state], ch);
            if (nextState == INVALID) {
                nextState = stateTable.size();
                stateTable[state].push_back({ch, nextState});
//console.log("B: State %d {ch: %d, nextState: %d}", state, ch, table.length);
                state = nextState; // Go to next state
                stateTable.push_back(std::vector<Transition>());
            } else {
                state = nextState ;
            }
        }
    }

auto end = std::chrono::steady_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//std::cout << "loadDictionary time = " << duration.count() << std::endl;

//std::cout << "stateTable size = " << stateTable.size() << std::endl;
//std::cout << "root transition count = " << stateTable[initialState].size() << std::endl;
}

/**
 * The general approach is from http://www.cs.umd.edu/~gasarch/BLOGPAPERS/FKS.pdf
 * this is usually known in the literature as FKS Perfect Hashing.
 * The idea of FKS hashing is to arrange the hash table in two levels:
 *   * In level 1, hash n keys into m buckets by a universal hash function h. 
 *     Bi is the set of items hashed to the ith bucket. 
 *   * In level 2, construct a perfect hashing sized Bi^2 for each bucket Bi.
 *
 * The Universal Hash used is: h(k) = ((kx mod p) mod s
 *
 * FKS Hashing is, however, the *general* case, but has several key problems:
 * 1. Two hash evaluations are required:
 *      row = ((k*id mod p) mod s where s is the number of states
 *      column = ((k*id mod p) mod Si where Si the is number of buckets in row i
 * 2. The modulo operation is expensive
 * 3. The approach requires extra data to be stored in the row and column arrays
 *    that is required by the hash function.
 *
 * Because Aho Corasick state machines have a tree structure and the number of
 * valid transitions decreases rapidly as the leaves are approached then it is
 * possible to do perfect hashing on each row of the state transition table.
 * That observation allows one to simplify the FKS approach into a single hash.
 * In this case the row table size is the number of state transitions and the
 * table contains an offset into the value table plus the k and Si values for
 * the particular row. Because we are hashing every row of the state transition
 * table we know that id is the transition character/octet, which is < 256
 * thus the prime number p can be 257 so that 1 <= k < 257.
 *
 * From the FKS paper the *sufficient* condition for collision free hashing is
 * Si >= Bi^2 and to reduce use of mod we choose Si to be a power of two, so for
 * ((ki * ch) % p) % Si we can instead do ((ki * ch) % p) & (Si - 1)
 *
 * Note 1:
 * As the maximum required value of Si is 256 we observe however that:
 * [Bi: 1, Si: 1], [Bi: 2, Si: 4], [Bi: 3, 4, Si: 16], [Bi: 5, Si: 32],
 * [Bi: 6, 7, 8 Si: 64], [Bi: 9, 10, 11 Si: 128], [Bi: 12, 13, ..., 255 Si: 256]
 * and note that the sufficient condition adds a space penalty (though we know
 * we are guaranteed a collision free hash). In the algorithm below we actually
 * relax the sufficient condition, so for say Bi of 3 we first *try* an Si of 4
 * rather than 16, now it is possible that *may* collide however in very many
 * cases it may not and would result in a greatly compressed transition table.
 *
 * Intuitively one might expect probing on the non-sufficient condition of Si
 * to have a significant insert time penalty, because if we fail to find a ki
 * value for the non-sufficient Si value we have to iterate again using the next
 * power of two potential Si value until eventually we reach Si >= Bi^2, but
 * in practice some initial timings suggest that the performance penalty to
 * iterate though the non-sufficient Si values probing for a valid ki is low
 * and the table compression gain is significant, which may help look up speed.
 */
void Dictionary::createHashTable() {
//    std::cout << "Creating Hash Table" << std::endl;

    // Clear any previous hash table
    hashRow.clear();
    hashVal.clear();
    initialTransitions.fill(INVALID);

    const std::int32_t numOfStates = stateTable.size();
//    std::cout << "numOfStates " << numOfStates << std::endl;

auto start = std::chrono::steady_clock::now();

    std::int32_t offset = 0;

    // Number of rows = number of states, which we know, so reserve capacity.
    hashRow.reserve(numOfStates);
    for (auto i = 0; i < numOfStates; i++) {
        hashRow.push_back({INVALID, INVALID}); // Initialise offset and k_sminus1.

        const std::vector<Transition>& currentState = stateTable[i];
        const int Bi = currentState.size();
        if (Bi) {
            if (i == initialState) { // Copy initial transitions into separate table.
                for (auto transition : currentState) {
                    initialTransitions[transition.ch] = transition.nextState;
                }
            } else { // Populate hashRow and hashVal.
                // Compute Si, which is the power of two greater than Bi
                std::int32_t Si = 256;
                for (const auto Bi2 = Bi * 2; Si >= Bi2; Si >>= 1) {/*empty*/}
                std::int32_t sminus1 = Si - 1;

                // Iteratively compute ki where ki is the lowest ki < P257 such
                // that no collisions occur for ((ki * ch) % p) % Si for all ch
                // For the case of Si = 1 or 256 we know ki will be 1.
                std::int32_t ki = INVALID;
                while (ki < 0) {
                    if (Si == 1 || Si == 256) {
                        ki = 1;
                    } else {
                        for (std::int32_t k = 1; k < P257 && ki < 0; k++) {
                            ki = k;
                            std::vector<bool> bits(Si, false);

                            // Check if this value of k has any collisions.
                            for (auto transition : currentState) {
                                const auto p = mod257(k * transition.ch) & sminus1;
                                if (bits[p]) {
                                    ki = INVALID; // Collision occurred, try next k.
                                    break;
                                } else {
                                    bits[p] = true;
                                }
                            }
                        }

                        if (ki == INVALID) { // No collision-free ki found, try next Si.
                            Si <<= 1;
                            sminus1 = Si - 1;
                        }
                    }
                }

                // Reserve next block of space in hashVal table.
                for (auto j = 0; j < Si; j++) {
                    hashVal.push_back({INVALID, INVALID});
                }

                for (auto transition : currentState) {
                    const auto p = mod257(ki * transition.ch) & sminus1;
                    hashVal[offset + p] = transition;
                }

                hashRow[i] = {offset, sminus1 | (ki << MASKBITS)};
                offset += Si;
            }
        }
    }

auto end = std::chrono::steady_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//std::cout << "createHashTable time = " << duration.count() << std::endl;
//std::cout << "hashRow size = " << hashRow.size() << std::endl;
//std::cout << "hashVal size = " << hashVal.size() << std::endl;
}

} // namespace gimbatuluk

