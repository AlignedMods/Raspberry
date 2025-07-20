#pragma once

#include <cstdint>

template<typename T>
T Combine(T first, T second) {
    T end = first + (UINT64_MAX * second);

    return end;
}
