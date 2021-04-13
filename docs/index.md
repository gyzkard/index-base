<!---
Copyright 2021-today gyzkard
(gyzkard@gyzkard.com)

Distributed under the MIT License.
(https://opensource.org/licenses/mit-license.php)
-->

index-base
==========

Strong types to distinguish between zero, one or maybe even other numbers based indexes.

## Copyright and License

* Copyright 2021-today gyzkard (gyzkard@gyzkard.com)

Distributed under the MIT License 
(https://opensource.org/licenses/mit-license.php).


## Basic Usage

```cpp
#include <inba/inba.hpp>

using namespace inba;

int data[] = { 2, 3 };
int sum = 0;
for (auto i = 1_i1; i <= Indexed_i1{ sizeof(data)/sizeof(int) }; ++i)
	sum += data[i]*static_cast<int>(i);

assert(sum == 3); // 2*0 + 3*1
```

## Predefined types and literals

User defined literals and typedef are defined for all combinations of built-in numeric types and base values zero and one.

They're are defined using the macro \ref INDEX_BASE_DEFINE_ALL at the end of \ref inba.hpp.

You can opt out of this by defining \ref INDEX_BASE_DISABLE_DEFINE_LITERALS or \ref INDEX_BASE_DISABLE_DEFINE_USINGS before including \ref inba.hpp.


F.e. for int and base zero, that's type `Indexed_i0` and literal `_i0`.

These are all the types:
- `signed char`, ci
- `unsigned char`, uci
- `signed short`, si
- `unsigned short`, usi
- `signed int`, i
- `unsigned int`, ui
- `signed long`, l
- `unsigned long`, ul
- `signed long long`, ll
- `unsigned long long`, ull
- `std::ptrdiff_t`, z
- `std::size_t`, uz
