#pragma once

#include "glew/glew.h"
#include "glew/wglew.h"

class   GLContext
{
protected:
    int         _format;
    //! 窗口句柄
    HWND        _hWnd;
    //! 绘制设备上下文
    HDC         _hDC;
    //! OpenGL上下文
    HGLRC       _hRC;
    bool        _init;
public:
    GLContext()
    {
        _format     =   0;
        _hWnd       =   0;
        _hDC        =   0;
        _hRC        =   0;
        _init       =   false;
    }

    ~GLContext()
    {
        shutdown();
    }
    /**
    *   初始化GL
    */
    bool    setup( HWND hWnd,HDC hDC )
    {
        _hWnd   =   hWnd;
        _hDC    =   hDC;
        unsigned PixelFormat;
        PIXELFORMATDESCRIPTOR pfd = 
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1, 
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32, 
            0,
            0,
            0, 
            0,
            0,
            0, 
            0,
            0,
            0, 
            0, 
            0, 
            0, 
            0, 
            24,
            8, 
            0,
            PFD_MAIN_PLANE, 
            0,
            0, 
            0, 
            0
        };
        if(_format == 0)
        {
            PixelFormat =   ChoosePixelFormat( _hDC, &pfd );
        }
        else
        {
            PixelFormat	=	_format;
        }
        if(!SetPixelFormat( _hDC, PixelFormat, &pfd ))
        {
            return  false;
        }
        _hRC        =   wglCreateContext( _hDC );
        if(!wglMakeCurrent( _hDC, _hRC ))
        {
            return  false;
        }
        glewInit();

        int     formt   =   setupMultisample();

        if( _hRC != NULL )
        {
            wglMakeCurrent( _hDC, NULL );
            wglDeleteContext( _hRC );
            _hRC = NULL;							
        }
        
        if(!SetPixelFormat( _hDC, formt, &pfd ))
        {
            return  false;
        }
        _hRC    =   wglCreateContext( _hDC );
        if(!wglMakeCurrent( _hDC, _hRC ))
        {
            return  false;
        }
        return  true;
    }


    int setupMultisample()
    {  
        if (wglChoosePixelFormatARB == 0)
        {
            return  0;
        }

        int		pixelFormat;
        int		valid;
        UINT	numFormats;
        float	fAttributes[] = {0,0};

        // These Attributes Are The Bits We Want To Test For In Our Sample
        // Everything Is Pretty Standard, The Only One We Want To 
        // Really Focus On Is The SAMPLE BUFFERS ARB And WGL SAMPLES
        // These Two Are Going To Do The Main Testing For Whether Or Not
        // We Support Multisampling On This Hardware.
        int iAttributes[] =
        {
            WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
            WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB,24,
            WGL_ALPHA_BITS_ARB,8,
            WGL_DEPTH_BITS_ARB,16,
            WGL_STENCIL_BITS_ARB,0,
            WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
            WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
            WGL_SAMPLES_ARB,4,
            0,0
        };

        valid = wglChoosePixelFormatARB(_hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);

        if (valid && numFormats >= 1)
        {
            return  pixelFormat;
        }
        iAttributes[19] = 2;
        valid = wglChoosePixelFormatARB(_hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
        if (valid && numFormats >= 1)
        { 
            return pixelFormat;
        }
        return  0;
    }


    /**
    *   销毁EGL
    */
    void    shutdown()
    {
        if( _hRC != NULL )
        {
            wglMakeCurrent( NULL, NULL );
            wglDeleteContext( _hRC );
            _hRC = NULL;							
        }
        if( _hDC != NULL )
        {
            ReleaseDC( _hWnd, _hDC );
            _hDC = NULL;
        }
    }

    /**
    *   交换缓冲区
    */
    void    swapBuffer()
    {
        SwapBuffers(_hDC);
    }

};
