#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#include <cassert>
#include <thread>
#include <chrono>
using namespace std::literals;
#include <system_error>

#include "bitmap.h"
#include "window.h"
#include "mandlebrot.h"

std::atomic_bool    done        {false};

namespace
{
HWND                theWindow   {};
constexpr int       WM_REFRESH  {WM_APP};
constexpr auto      windowStyle { WS_OVERLAPPED | WS_CAPTION  |  WS_SYSMENU  | WS_MINIMIZEBOX | WS_VISIBLE    };


void paint(HWND h,WPARAM w, LPARAM l)
{
    PAINTSTRUCT paint;
    BeginPaint(h,&paint);

    RECT  r{};
    GetClientRect(h,&r);
    
    StretchDIBits(paint.hdc,
                  0,0,
                  r.right-r.left,
                  r.bottom-r.top,
                  0,0,
                  dim,dim,
                  bitmapData,
                  bitmapHeader,
                  DIB_RGB_COLORS,
                  SRCCOPY);

    
    EndPaint(h,&paint);
}

LRESULT CALLBACK proc(HWND h, UINT m, WPARAM w, LPARAM l)
{
    static  int downX, downY;

    switch(m)
    {
    case WM_CREATE:
    {        
        RECT    client{0,0, dim, dim};

        AdjustWindowRect(&client,windowStyle,FALSE);
        SetWindowPos(h, nullptr,0,0, client.right-client.left, client.bottom-client.top, SWP_NOMOVE|SWP_NOZORDER);
        GetClientRect(h,&client);
        SetForegroundWindow(h);

        return 0;
    }

    case WM_CLOSE:
        done=true;
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        paint(h,w,l);
        return 0;

    case WM_REFRESH:
        InvalidateRect(h,nullptr,FALSE);
        return 0;

    case WM_LBUTTONDOWN:

        downX = GET_X_LPARAM(l);
        downY = GET_Y_LPARAM(l);

        break;

    case WM_LBUTTONUP:
    {
        
        auto upX = GET_X_LPARAM(l);
        auto upY = GET_Y_LPARAM(l);

        if(   upX != downX
           && upY != downY)
        {
            stop();

            auto topLeft     = fromPixel( downY,downX);
            auto bottomRight = fromPixel( upY,upX);

            go(topLeft,bottomRight);

        }
    
        break;
    }
    
    case WM_NCHITTEST:
    case WM_MOUSEMOVE:
    case WM_NCMOUSEMOVE:
    case WM_SETCURSOR:
        break;

    default:
        //print("msg {:#x}\n",m);
        break;
    }

    return DefWindowProc(h,m,w,l);
}


}


void createWindow()
{
    WNDCLASSA    Class
    {
        CS_OWNDC,
        proc,
        0,
        0,
        GetModuleHandle(nullptr),
        nullptr,
        LoadCursor(nullptr,IDC_ARROW),
        nullptr,
        nullptr,
        "mandlebrot"
    };

    if(!RegisterClassA(&Class))
    {
        throw std::system_error{ static_cast<int>(GetLastError()), std::system_category(), "RegisterClass"};
    }

    theWindow = CreateWindowA(Class.lpszClassName,
                              "Mandlebrot",
                              windowStyle,
                              0,0,
                              10,10,
                              nullptr,
                              nullptr,
                              GetModuleHandle(nullptr),
                              nullptr);

    if(theWindow==nullptr)
    {
        throw std::system_error{ static_cast<int>(GetLastError()), std::system_category(), "RegisterClass"};
    }
}



void windowMessageLoop()
{
    MSG     msg;
    while(GetMessage(&msg,0,0,0) > 0)
    {
        DispatchMessage(&msg);
    }
}


void redrawWindow()
{
    SendMessage(theWindow,WM_REFRESH,0,0);
}

void setTitle(std::string const &title)
{
    SetWindowText(theWindow,title.c_str());
}