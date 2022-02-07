#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstddef>
#include <utility>
#include <mutex>
#include <cassert>

#include "bitmap.h"


namespace
{


RGBQUAD fromHCXM(double Hprime, double chroma, double X, double m)
{
    RGBQUAD rgb{};

    auto scale = [&](double d)
    {
        return static_cast<BYTE>(256*(d+m));
    };

    if(Hprime < 1)
    {
        rgb.rgbRed   = scale(chroma);
        rgb.rgbGreen = scale(X); 
        rgb.rgbBlue  = scale(0); 
    }
    else if(Hprime < 2)
    {
        rgb.rgbRed   = scale(X); 
        rgb.rgbGreen = scale(chroma);
        rgb.rgbBlue  = scale(0); 
    }
    else if(Hprime < 3)
    {
        rgb.rgbRed   = scale(0); 
        rgb.rgbGreen = scale(chroma);
        rgb.rgbBlue  = scale(X); 
    }
    else if(Hprime < 4)
    {
        rgb.rgbRed   = scale(0); 
        rgb.rgbGreen = scale(X); 
        rgb.rgbBlue  = scale(chroma);
    }
    else if(Hprime < 5)
    {
        rgb.rgbRed   = scale(X); 
        rgb.rgbGreen = scale(0); 
        rgb.rgbBlue  = scale(chroma);
    }
    else 
    {
        rgb.rgbRed   = scale(chroma);
        rgb.rgbGreen = scale(0); 
        rgb.rgbBlue  = scale(X);
    }

    return rgb;
}


RGBQUAD fromHSL(double H, double S, double L)
{
    assert( H>= 0 && H <= 1);
    assert( S>= 0 && S <= 1);
    assert( L>= 0 && L <= 1);

    auto Hprime = H*6;
    auto chroma = (1 - std::abs(2 * L - 1)) * S;
    auto X      = chroma * (1 - std::abs (std::fmod(Hprime,2)  -1 ))  ;
    auto m      = L - chroma/2;

    return fromHCXM(Hprime,chroma,X,m);
}



auto makeHeader()
{
    auto header = reinterpret_cast<BITMAPINFO*>( new  std::byte[ sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)]);

    header->bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);
    header->bmiHeader.biWidth           =  dim;
    header->bmiHeader.biHeight          = -dim;
    header->bmiHeader.biPlanes          =    1;
    header->bmiHeader.biBitCount        =    8;
    header->bmiHeader.biCompression     = BI_RGB;
    header->bmiHeader.biSizeImage       = dim*dim,
    header->bmiHeader.biXPelsPerMeter   = 0;
    header->bmiHeader.biYPelsPerMeter   = 0;
    header->bmiHeader.biClrUsed         = 0;
    header->bmiHeader.biClrImportant    = 0;

    header->bmiColors[0]= RGBQUAD{};

    for(int i=1;i<256;i++)
    {
        header->bmiColors[i]= fromHSL(i/256.0,.99,0.5);
    }

    return header;
}

}


BITMAPINFO     *bitmapHeader        {makeHeader()};
uint8_t         bitmapData[dim][dim]{};
//                               ^---- must be multiple of 4





