/*
 * Copyright 2017 Henry Elliott
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "random_device.hh"
#include <iostream>
#include <random>
#include <chrono>

static void test_random_device()
{
	std::random_device rd;
	for (int i = 0; i != 999999999; ++i) {
		rd();
	}
	std::cout << rd() << '\n';
}

static void test_my_random_device()
{
	cout_hello_world::random_device rd;
	for (int i = 0; i != 999999999; ++i) {
		rd();
	}
	std::cout << rd() << '\n';
}

int main()
{
	auto t0 = std::chrono::steady_clock::now();
	test_random_device();
	auto t1 = std::chrono::steady_clock::now();
	test_my_random_device();
	auto t2 = std::chrono::steady_clock::now();
	auto i0 = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();
	auto i1 = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count();
	std::cout << "test_random_device() took " << i0 << "s\ntest_my_random_device() took "
	          << i1 << "s\n";
}
