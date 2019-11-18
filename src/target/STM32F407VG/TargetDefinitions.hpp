#pragma once

#include "stm32f4xx.h"

using ValueType = uint32_t;

constexpr ValueType VectorsCount    = 82 + 16;  // 82 mutable + 16 immutable vectors
constexpr ValueType Alignment       = 256;      // The cortex M-x vector tables need to be specifically aligned in RAM
