#pragma once

#include "GLContext.h"

class   OpenGLWindow :public GLContext
{
protected:
    int     _width;
    int     _height;
public:

    OpenGLWindow(HINSTANCE hInst = 0)
    {
        //1 注册窗口类
        WNDCLASSEX  wnd;

        wnd.lpszClassName  =   ("lesson01");
        wnd.cbSize         =   sizeof(WNDCLASSEX);
        wnd.style          =   CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wnd.lpfnWndProc    =   wndProc;
        wnd.hInstance      =   0;
        wnd.hIcon          =   0;
        wnd.hIconSm        =   0;
        wnd.hCursor        =   LoadCursor(NULL, IDC_ARROW);
        wnd.hbrBackground  =   (HBRUSH)GetStockObject(BLACK_BRUSH);
        wnd.lpszMenuName   =   NULL;
        wnd.cbClsExtra     =   0;
        wnd.cbWndExtra     =   0;

        RegisterClassEx(&wnd);
    }

    virtual void    onInitGL()
    {
    }

    virtual void    onShutdownGL()
    {
    }

    virtual HWND    createWindow(int w, int h)
    {
        //2 创建窗口
        HWND hWnd = CreateWindowEx(
            0
            , ("lesson01")
            , ("窗口标题")
            , WS_OVERLAPPEDWINDOW
            , 10
            , 10
            , w
            , h
            , 0
            , 0
            , 0
            , this);

        return  hWnd;
    }

    virtual bool    main(int w,int h)
    {
        _width  =   w;
        _height =   h;
        _hWnd    = createWindow(w,h);

        if (!setup(_hWnd,GetDC(_hWnd)))
        {
            return  false;
        }
        onInitGL();
        //!3    更新显示
        if (_hWnd)
        {
            UpdateWindow(_hWnd);
            ShowWindow(_hWnd,SW_SHOW);
        }
        else
        {
            return  false;
        }
        MSG     msg =   {0};

        //4 消息循环
        while(WM_QUIT != msg.message) 
        {
            if(PeekMessage(&msg, 0, 0, 0,PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } 
            else
            {
                RECT    rect;
                GetClientRect(_hWnd,&rect);

                _width  =   rect.right - rect.left;
                _height =   rect.bottom - rect.top;

                glClearColor(0,0,0,1);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glViewport(0,0,_width,_height);

                render();

                swapBuffer();
            }
        }

        onShutdownGL();
        shutdown();
        return  true;
    }

    virtual void    render()
    {
    }

    /**
    *   消息处理过程
    */
    virtual LRESULT events(HWND hWnd,UINT msgId, WPARAM wParam, LPARAM lParam)
    {
        switch(msgId)
        {
        case WM_CLOSE:
        case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
            break;
        default:
            return DefWindowProc(hWnd, msgId, wParam, lParam);
        }
        return  0;
    }
protected:
    static  LRESULT CALLBACK  wndProc(HWND hWnd, UINT msgId, WPARAM wParam, LPARAM lParam)
    {
        if (msgId == WM_CREATE)
        {
            LPCREATESTRUCT  pData   =   (LPCREATESTRUCT)lParam;
            OpenGLWindow*   pTHis   =   (OpenGLWindow*)pData->lpCreateParams ;
            SetWindowLong(hWnd,GWL_USERDATA,(LONG)pTHis);
            return DefWindowProc(hWnd, msgId, wParam, lParam);
        }
        else
        {
            OpenGLWindow*   pTHis   =   (OpenGLWindow*)GetWindowLong(hWnd,GWL_USERDATA);
            if (pTHis)
            {
                return  pTHis->events(hWnd,msgId,wParam,lParam);
            }
            else
            {
                return DefWindowProc(hWnd, msgId, wParam, lParam);
            }
        }
    }
};