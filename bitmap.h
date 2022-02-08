#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>

constexpr int dim{800};

extern BITMAPINFO     *bitmapHeader;
extern uint8_t         bitmapData[dim][dim];


void palette();