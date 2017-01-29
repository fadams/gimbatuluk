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
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include <iostream>

namespace gimbatuluk {

template<typename T, std::size_t N>
class CircularStore {
public:
    CircularStore(): head(0), tail(0) {
        for (auto i = 0u; i < N; i++) {
            release(value[i]);
        }
    }

    ~CircularStore() {
        head = -1;
        tail = -1;
    }

    CircularStore(CircularStore&&) = delete;
    CircularStore(const CircularStore&) = delete;
    CircularStore& operator=(CircularStore&&) = delete;
    CircularStore& operator=(const CircularStore&) = delete;

    T& get() {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock, [&]{return head != tail;});
        head = (head == N) ? 0 : head + 1;
        return value[index[head]];
    }

    void release(const T& callback) {
        if (tail == -1) return;
        std::unique_lock<std::mutex> lock(mutex);
        tail = (tail == N) ? 0 : tail + 1;
        index[tail] = &callback - &value[0];
        lock.unlock();
        cond.notify_one();
    }
private:
    std::array<int, N + 1> index;
    std::array<T, N> value;

    int head;
    int tail;
    std::mutex mutex;
    std::condition_variable cond;
};

} // namespace gimbatuluk

