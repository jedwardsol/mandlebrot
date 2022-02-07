#include <Windows.h>
#include <vector>

#include "bitmap.h"
#include "window.h"


#include <thread>
#include <complex>

std::thread  theThread;

std::complex<double>  topLeft;
std::complex<double>  bottomRight;


std::complex<double>  fromPixel(int row, int column)
{
    return {  topLeft.real()  + column  * (bottomRight.real() -topLeft.real() ) / dim,
              topLeft.imag()  + row     * (bottomRight.imag() -topLeft.imag() ) / dim };
}


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
            auto c = fromPixel(row, column);


            bitmapData[row][column] = iterate(c);

            if(done)
            {
                return;
            }

        }

        redrawWindow();
    }
}


void go(std::complex<double> const &topLeft,std::complex<double> const &bottomRight)
{
    auto title = std::format("Mandlebrot {},{}  to  {},{}",topLeft.real(), topLeft.imag(), bottomRight.real(), bottomRight.imag());

    setTitle(title);

    ::topLeft=topLeft;
    ::bottomRight=bottomRight;

    theThread = std::thread{mandlebrot};
}

void stop()
{
    done=true;
    theThread.join();
    done=false;
}

int main()
{
    createWindow();

    go( {-2.0, 1.0}, { 0.5, -1.0} );

  
    windowMessageLoop();

    stop();
}