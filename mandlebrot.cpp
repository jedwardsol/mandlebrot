#include <Windows.h>
#include <vector>

#include "bitmap.h"
#include "window.h"


#include <thread>
#include <complex>


double left   {-2.0};
double right  { 0.5};

double top    { 1.0};
double bottom {-1.0};


int iterate(std::complex<double> const &c)
{
    std::complex<double>  z{0,0};

    int count{};


    while(count < 256)
    {
        z= z*z + c;
        count++;            

        if(std::abs(z) > 2.0)
        {
            return count;
        }
    }

    return 0;

}

void mandlebrot()
{
    for(int row=0;row<dim;row++)
    {
        for(int column=0;column<dim;column++)
        {
            std::complex<double>    c{  left + column  * (right-left) / dim,
                                        top  + row     * (bottom-top) / dim };

            bitmapData[row][column] = iterate(c);
            redrawWindow();
        }
    }
}


int main()
{
    createWindow();

    std::thread{mandlebrot}.detach();

    windowMessageLoop();
}