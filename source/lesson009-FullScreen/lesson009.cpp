
#include <windows.h>
#include <tchar.h>
#include "GLESContext.h"

LRESULT CALLBACK  wndProc(HWND hWnd, UINT msgId, WPARAM wParam, LPARAM lParam)
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

void    ChangeToFullScreen(int w,int h)
{
    DEVMODE dmSettings  =   {0};

    /**
    *   枚举设备模式
    */
    if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
    {
        MessageBox(NULL, _T("Could Not Enum Display Settings"), _T("Error"), MB_OK);
        return;
    }

    dmSettings.dmPelsWidth	= w;
    dmSettings.dmPelsHeight	= h;
    dmSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

    if(result != DISP_CHANGE_SUCCESSFUL)
    {
        MessageBox(NULL, _T("Display Mode Not Compatible"), _T("Error"), MB_OK);
        PostQuitMessage(0);
    }
}

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
    //1 注册窗口类
    WNDCLASSEX  wnd;

    memset(&wnd,0,sizeof(wnd));

    wnd.cbSize          =   sizeof(wnd);
    wnd.lpfnWndProc     =   wndProc;
    wnd.hbrBackground   =   (HBRUSH)GetStockObject(GRAY_BRUSH);
    wnd.hCursor         =   LoadCursor(hInstance,IDC_ARROW);
    wnd.hIcon           =   0;
    wnd.hIconSm         =   0;
    wnd.hInstance       =   hInstance;
    wnd.lpszClassName   =   _T("lesson01");
    wnd.style           =   CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    RegisterClassEx(&wnd);

    //2 创建窗口
    HWND hWnd   =   CreateWindowEx(
        0
        , _T("lesson01") 
        , _T("窗口标题") 
        , WS_POPUP|WS_VISIBLE
        , 0
        , 0
        , 800
        , 600
        , 0
        , 0
        ,hInstance
        , 0);

    ChangeToFullScreen(800,600);
    //!3    更新显示
    if (hWnd)
    {
        UpdateWindow(hWnd);
        ShowWindow(hWnd,SW_SHOW);
    }

    else
    {
        return  0;
    }

    GLESContext   context;

    if (!context.setup(hWnd,GetDC(hWnd)))
    {
        return  0;
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
            /**
            *   调用绘制函数进行绘制
            */
            glClearColor(1,0,0,1);
            glClear(GL_COLOR_BUFFER_BIT);
            context.swapBuffer();

        }
    }

    context.shutdown();

    return  0;
}