#pragma once
#include <cstdint>

// Display resolution
constexpr int EPD_2IN9_V2_WIDTH = 128;
constexpr int EPD_2IN9_V2_HEIGHT = 296;

class EPaper_API
{
    public:
    virtual void Reset() = 0;
    virtual void Init() = 0;
    virtual void Clear() = 0;
    virtual void Display(uint8_t *image) = 0;
    virtual void DisplayBase(uint8_t *image) = 0;
    virtual void DisplayPartial(uint8_t *image) = 0;
    virtual void Sleep() = 0;
};

