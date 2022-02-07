#include <Windows.h>
#include <vector>
#include <array>
#include <string>

#include "bitmap.h"
#include "window.h"


#include <thread>
#include <complex>
#include "mandlebrot.h"

constexpr int                       numThreads{6};
std::array<std::thread,numThreads>  theThreads;
Point                               topLeft;
Point                               bottomRight;


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

void mandlebrot(int startRow)
{
    for(int row=startRow;row<dim;row+=numThreads)
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
    using std::to_string;

    auto title = std::format("Mandlebrot {},{} to  {},{}",to_string(topLeft.real()),     to_string(topLeft.imag()), 
                                                          to_string(bottomRight.real()), to_string(bottomRight.imag()));

    setTitle(title);

    ::topLeft=topLeft;
    ::bottomRight=bottomRight;

    memset(bitmapData,0,sizeof(bitmapData));

    for(int i=0;i<numThreads;i++)
    {
        theThreads[i] = std::thread{mandlebrot,i};
    }
}

void stop()
{
    done=true;
    
    for(auto &thread : theThreads)
    {
        thread.join();
    }
    done=false;
}

int main()
{
    createWindow();

    go( {-2.0, 1.0}, { 0.5, -1.0} );

  
    windowMessageLoop();

    stop();
}