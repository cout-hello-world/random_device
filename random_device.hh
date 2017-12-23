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
#ifndef COUT_HELLO_WORLD_RANDOM_DEVICE
#define COUT_HELLO_WORLD_RANDOM_DEVICE
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <climits>
#include <stdexcept>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#ifdef __linux__
  #include <linux/random.h>
#endif
namespace cout_hello_world {
template<std::size_t CallsPerBuffer = 1024>
class basic_random_device {
public:
	using result_type = unsigned int;
	static_assert(CHAR_BIT == 8, "CHAR_BIT must equal 8.");
	static constexpr result_type min()
	{
		return std::numeric_limits<result_type>::min();
	}
	static constexpr result_type max()
	{
		return std::numeric_limits<result_type>::max();
	}

	double entropy() const noexcept
	{
#ifdef __linux__
		int entropy;
		if (ioctl(fd, RNDGETENTCNT, &entropy) == -1) {
			return -1.0;
		}
		return static_cast<double>(entropy);
#else
		return -1.0;
#endif
	}

	basic_random_device(const std::string &token = "/dev/urandom")
	{
		fd = open(token.c_str(), O_RDONLY);
		if (fd == -1) {
			throw std::runtime_error(
			    "character device open failed");
		}
	}

	result_type operator()()
	{
		if (offset == BufferSize) {
			auto bytes = decltype(read(fd, buffer, BufferSize)){0};
			while (bytes != BufferSize) {
				auto ret = read(fd, buffer + bytes,
				                BufferSize - bytes);
				if (ret == -1) {
					throw std::runtime_error(
					    "character device read failed");
				}
				bytes += ret;
			}
			offset = 0;
		}
		result_type result{0};
		for (auto i = decltype(offset){0}; i != sizeof(result_type);
		     ++i) {
			result |= static_cast<result_type>(buffer[offset + i])
			          << (i * CHAR_BIT);
		}
		offset += sizeof(result_type);
		return result;
	}

	~basic_random_device()
	{
		close(fd);
	}

	basic_random_device(const basic_random_device &) = delete;
	basic_random_device &operator=(const basic_random_device &) = delete;
private:
	static constexpr std::size_t BufferSize = CallsPerBuffer *
	                                          sizeof(result_type);
	std::uint8_t buffer[BufferSize];
	std::size_t offset = BufferSize;
	int fd;
};

using random_device = basic_random_device<>;
}
#endif
