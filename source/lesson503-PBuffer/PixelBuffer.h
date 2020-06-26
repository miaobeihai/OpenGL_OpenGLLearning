#pragma once

#include "glew/glew.h"
#include "glew/wglew.h"
class   PixelBuffer
{
public:
    HPBUFFERARB _hPBuffer;
    HDC         _hDC;
    HWND        _hWnd;
    HGLRC       _hRC;
    int         _width;
    int         _height;
public:
    /**
    *   ½¨Á¢
    */
    bool    setup(HWND hWnd, HDC hDC, HGLRC hRC,int w, int h)
    {
        _hWnd   =   hWnd;
        _width  =   w;
        _height =   h;

        int pf_attr[] =
        {
            WGL_SUPPORT_OPENGL_ARB, TRUE,       // P-buffer will be used with OpenGL
            WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // Enable render to p-buffer
            WGL_RED_BITS_ARB, 8,                // At least 8 bits for RED channel
            WGL_GREEN_BITS_ARB, 8,              // At least 8 bits for GREEN channel
            WGL_BLUE_BITS_ARB, 8,               // At least 8 bits for BLUE channel
            WGL_ALPHA_BITS_ARB, 8,              // At least 8 bits for ALPHA channel
            WGL_DEPTH_BITS_ARB, 16,             // At least 16 bits for depth buffer
            WGL_DOUBLE_BUFFER_ARB, FALSE,       // We don't require double buffering
            0                                   // Zero terminates the list
        };

        unsigned    count = 0;
        int         pixelFormat;
        wglChoosePixelFormatARB(hDC, (const int*)pf_attr, NULL, 1, &pixelFormat, &count);

        if (count == 0)
        {
            return  false;
        }

        _hPBuffer   =   wglCreatePbufferARB(hDC, pixelFormat, _width, _height, NULL);
        _hDC        =   wglGetPbufferDCARB(_hPBuffer);
        _hRC        =   wglCreateContext(_hDC);

        if (_hPBuffer == 0)
        {
            return  false;
        }

        wglQueryPbufferARB(_hPBuffer, WGL_PBUFFER_WIDTH_ARB, &w);
        wglQueryPbufferARB(_hPBuffer, WGL_PBUFFER_HEIGHT_ARB, &h);

        if (h != _height || w != _width)
        {
            return  false;
        }


        if (!wglMakeCurrent(_hDC, _hRC))
        {
            return  false;
        }


        if (!wglShareLists(hRC, _hRC))
        {
            return  false;
        }

        return  true;
    }

    void    makeCurrent()
    {
        wglMakeCurrent(_hDC, _hRC);
    }

    void    end()
    {

    }

    /**
    *   Ïú»Ù
    */
    void    destroy()
    {
        if (_hRC != NULL)
        {
            wglMakeCurrent(_hDC, _hRC);
            wglDeleteContext(_hRC);
            wglReleasePbufferDCARB(_hPBuffer, _hDC);
            wglDestroyPbufferARB(_hPBuffer);
            _hRC = NULL;
        }
        if (_hDC)
        {
            ReleaseDC(_hWnd, _hDC);
            _hDC = 0;
        }
    }
};