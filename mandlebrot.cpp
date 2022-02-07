#include <Windows.h>
#include <vector>

#include "bitmap.h"
#include "window.h"


#include <thread>
#include <complex>
#include "mandlebrot.h"

std::thread  theThread;

Point  topLeft;
Point  bottomRight;


Point  fromPixel(int row, int column)
{
    return {  topLeft.real()  + column  * (bottomRight.real() -topLeft.real() ) / dim,
              topLeft.imag()  + row     * (bottomRight.imag() -topLeft.imag() ) / dim };
}


int iterate(Point const &c)
{
    Point  z{0,0};

    int count{};

    while(count < 256)
    {
        z= z*z + c;
        count++;            

        if(abs(z) > 2.0)
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


void go(Point const &topLeft,Point const &bottomRight)
{
    auto title = "Mandlebrot";//std::format("Mandlebrot {}  to  {}",topLeft.real(), topLeft.imag(), bottomRight.real(), bottomRight.imag());

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