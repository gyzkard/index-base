# index-base

Strong types for C++ to distinguish between zero, one or other numbers based indexes.

## Quickstart

```c++
#include <inba/inba.hpp>

using namespace inba;

int data[] = { 2, 3 };
int sum = 0;
for (auto i = 1_i1; i <= Indexed_i1{ sizeof(data)/sizeof(int) }; ++i)
	sum += data[i]*static_cast<int>(i);

assert(sum == 3); // 2*0 + 3*1
```

## Supported compilers

At the moment, this library requires C++11 support.
It has been successfully tested with
* MSVC V16.9.2 (VS 2019) V15.9.34 (VS 2017), mostly V14.0.25431.01 Update 3 (VS 2015) [some traits don't work yet]; VS 2010 and below won't work
* clang 7.0.1-8 and 11.0.1-2
* gcc 8.3.0-6 and 10.2.1-6
