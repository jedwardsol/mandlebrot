#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>


constexpr int dim{1000};

// palette entries
enum Colour
{
    set         = 255,

};


extern BITMAPINFO     *bitmapHeader;
extern uint8_t         bitmapData[dim][dim];

