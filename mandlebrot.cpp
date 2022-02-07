#include <Windows.h>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <stack>

#include "bitmap.h"
#include "window.h"


#include <thread>
#include <complex>
#include "mandlebrot.h"

constexpr int                           numThreads{6};
std::array<std::thread,numThreads+1>    theThreads;


struct Box
{
    Point                               topLeft;
    Point                               bottomRight;
};

std::stack<Box> history;


Point  fromPixel(int row, int column)
{
    auto &current=history.top();

    return {  current.topLeft.real()  + column  * (current.bottomRight.real() -current.topLeft.real() ) / dim,
              current.topLeft.imag()  + row     * (current.bottomRight.imag() -current.topLeft.imag() ) / dim };
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



void mandlebrotLowRes()
{
    constexpr int lowResolution{10};
    static_assert((dim % lowResolution) == 0);

    for(int row=lowResolution;row<dim;row+=lowResolution)
    {
        for(int column=0;column<dim;column+=lowResolution)
        {
            auto c = fromPixel(row, column);

            bitmapData[row][column]   = iterate(c);

            for(int i=0;i<lowResolution;i++)
            {
                for(int j=0;j<lowResolution;j++)
                {
                    bitmapData[row+j][column+i] = bitmapData[row][column];
                }
            }

            if(done)
            {
                return;
            }
        }
    }
}



void go()
{
    using std::abs;

    auto &current=history.top();

    auto width  = abs(current.bottomRight.real() - current.topLeft.real());
    auto height = abs(current.bottomRight.imag() - current.topLeft.imag());

    auto stringise = [](auto const &val)
    {
        std::ostringstream  s;

        s<<val;

        return s.str();
    };


    auto title = std::format("Mandlebrot width {}  height {}",stringise(width),stringise(height));

    setTitle(title);



    memset(bitmapData,0,sizeof(bitmapData));

    theThreads[numThreads] = std::thread{mandlebrotLowRes};


    for(int i=0;i<numThreads;i++)
    {
        theThreads[i] = std::thread{mandlebrot,i};
    }

}

void go(Point const &topLeft,Point const &bottomRight)
{

    history.emplace(topLeft,bottomRight);

    go();
}


void goUp()
{
    if(history.size() > 1)
    {
        stop();
        history.pop();
        go();
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