# random_device
This is a header-only drop-in replacement for `std::random_device`. It requires
a POSIX operating system, and the `entropy()` member function only returns
a meaningful value on Linux.

## Questions and Answers
### Why would I use this code?
According to my tests, this code (using `/dev/urandom` as a backend) produces
random numbers about six times faster than the `std::random_device` of
`libstdc++` on my platform (x86_64 Debian). This is achieved by reading from
the underlying character device (e.g. `/dev/urandom`) in bulk.

### How do I use this code?
The `random_device` is in the namespace `cout_hello_world`. Use it exactly as
you would a `std::random_device`. Furthermore, use the class template
`cout_hello_world::basic_random_device<std::size_t CallsPerBuffer>` to tune the number of calls
to `operator()` which can be made between reads to the underlying character
device. The memory used is approximately equal to
`sizeof(unsigned int) * CallsPerBuffer`.
`cout_hello_world::random_device` is defined in the header with
```
using random_device = basic_random_device<1024>;
```

### How do I integrate this code with my project?
If you’re using a POSIX OS, simply drop the header `random_device.hh` into your
project and go. Be aware that names from POSIX headers (and, on linux,
`<linux/random.h>`) will be placed in the global namespace.
