
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "FreeImage.h"


#include "CELLMath.hpp"

#include "OpenGLWindow.h"
#include "Camera3rd.h"
#include "Camera.h"
#include <vector>
using namespace CELL;

struct Vertex
{ 
    float x, y, z;
    float u, v;
};
class   SamplerTexture :public OpenGLWindow
{
    GLuint  _texture;

    GLuint  _texGround;

    Camera  _camera[3];
    int2    _mouseDown;
    bool    _bDownFlag;

    int     _cameraIndex;
    int     _cameraOld;
    float3  _rolePos;
    float3  _moveTo;


public:
    SamplerTexture() 
    {
        _bDownFlag  =   false;
        _rolePos    =   float3(0,0,0);
        _moveTo     =   _rolePos;
        _cameraIndex=   0;
        _cameraOld  =   0;

        _camera[0]._eye    =   float3(0,10,10);
        _camera[0]._lookat =   float3(0,0,0);


        _camera[1]._eye    =   float3(10,0,10);
        _camera[1]._lookat =   float3(-10,0,-10);


        _camera[2]._eye    =   float3(10,10,10);
        _camera[3]._lookat =   float3(-10,0,-10);


    }         
    /**
    *   使用FreeImage加载图片
    */
    unsigned    createTextureFromImage(const char* fileName)
    {
        //1 获取图片格式
        FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
        if (fifmt == FIF_UNKNOWN)
        {
            return  0;
        }
        //2 加载图片
        FIBITMAP    *dib = FreeImage_Load(fifmt, fileName,0);
     
        FREE_IMAGE_COLOR_TYPE type    =   FreeImage_GetColorType(dib);
      
        //! 获取数据指针
        FIBITMAP*   temp    =   dib;
        dib =   FreeImage_ConvertTo32Bits(dib);
                FreeImage_Unload(temp);

        BYTE*   pixels =   (BYTE*)FreeImage_GetBits(dib);
        int     width   =   FreeImage_GetWidth(dib);
        int     height  =   FreeImage_GetHeight(dib);

        for (int i = 0 ;i < width * height * 4 ; i+=4 )
        {
            BYTE temp       =   pixels[i];
            pixels[i]       =   pixels[i + 2];
            pixels[i + 2]   =   temp;
        }

        unsigned    res =   createTexture(width,height,pixels);
        FreeImage_Unload(dib);
        return      res;
    }

    unsigned    createTexture(int w,int h,const void* data)
    {
        unsigned    texId;
        glGenTextures(1,&texId);
        glBindTexture(GL_TEXTURE_2D,texId);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);

        return  texId;
    }

    virtual void    onInitGL()
    {
        _texture    =   createTextureFromImage("2.jpg");
        _texGround  =   createTextureFromImage("1.jpg");

    }
    virtual void    render()
    {
#define M_PI (3.14159265358979323846)
       

        float   size    =   100;

        Vertex cubeVertices[] =
        {
            {  -1.0f,-1.0f, 1.0f    ,0.3f,  0.3f },
            {  1.0f,-1.0f, 1.0f     ,0.5f,  0.3f },
            {  1.0f, 1.0f, 1.0f     ,0.5f,  0.8f },
            { -1.0f, 1.0f, 1.0f     ,0.3f,  0.8f },

            {  -1.0f,-1.0f,-1.0f    ,0,  0 },
            {  -1.0f, 1.0f,-1.0f    ,1,  0 },
            {  1.0f, 1.0f,-1.0f     ,1,  1 },
            {  1.0f,-1.0f,-1.0f     ,0,  1 },

            {  -1.0f, 1.0f,-1.0f    ,0,  0 },
            {  -1.0f, 1.0f, 1.0f    ,1,  0 },
            {  1.0f, 1.0f, 1.0f     ,1,  1 },
            {  1.0f, 1.0f,-1.0f     ,0,  1 },

            {  -1.0f,-1.0f,-1.0f    ,0,  0 },
            {  1.0f,-1.0f,-1.0f     ,1,  0 },
            {  1.0f,-1.0f, 1.0f     ,1,  1 },
            {  -1.0f,-1.0f, 1.0f    ,0,  1 },

            {  1.0f,-1.0f,-1.0f ,0,  0 },
            {  1.0f, 1.0f,-1.0f ,1,  0 },
            {  1.0f, 1.0f, 1.0f ,1,  1 },
            {  1.0f,-1.0f, 1.0f ,0,  1 },

            {  -1.0f,-1.0f,-1.0f ,0,  0 },
            {  -1.0f,-1.0f, 1.0f ,1,  0 },
            {  -1.0f, 1.0f, 1.0f ,1,  1 },
            {  -1.0f, 1.0f,-1.0f ,0,  1 },

//! 地面数据

            {  -size, -1,    -size       ,0,  0 },
            {  -size, -1,    size        ,100,  0 },
            {  size,  -1,     size        ,100,  100 },
            {  size,  -1,     -size       ,0,  100 },
        };

        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_MODELVIEW);

        _camera[_cameraIndex].perspective(60,float(_width)/float(_height),0.1f,1000.0f);
        
       

        //! 摄像机更新
        _camera[_cameraIndex].update();

        matrix4 matVP  =   _camera[_cameraIndex].getMVP();
        glLoadMatrixf(matVP.data());
        
        //! 产生一个矩阵
        glTranslatef(_rolePos.x,_rolePos.y,_rolePos.z);

        glBindTexture(GL_TEXTURE_2D,_texture);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3,GL_FLOAT,         sizeof(Vertex),     &cubeVertices[0].x);
        glTexCoordPointer(2,GL_FLOAT,       sizeof(Vertex),     &cubeVertices[0].u);

        
        glDrawArrays( GL_QUADS, 0, 24 );

        glLoadMatrixf(matVP.data());

        glBindTexture(GL_TEXTURE_2D,_texGround);
        glDrawArrays(GL_QUADS,24,4);


    }

    virtual LRESULT events(HWND hWnd,UINT msgId, WPARAM wParam, LPARAM lParam)
    {
        switch (msgId)
        {
        case WM_SIZE:
            {
                if (::IsWindow(hWnd))
                {
                    RECT    rt;
                    GetClientRect(_hWnd,&rt);
                    int     w   =   rt.right - rt.left;
                    int     h   =   rt.bottom - rt.top;
                    glViewport(0,0,w ,h );
                    _camera[_cameraIndex].setView(w,h);
                }
            }
            break;
        case WM_LBUTTONDOWN:
            {
                _bDownFlag  =   true;
                _mouseDown  =   int2(LOWORD(lParam),HIWORD(lParam));
            }
            break;
        case WM_LBUTTONUP:
            {
                _bDownFlag  =   false;
            }
            break;

        case WM_MOUSEWHEEL:
            {
            }
            break;
        case WM_RBUTTONDOWN:
            {
                _cameraIndex    +=  1;
                _cameraIndex    %=  3;
            }
            break;

        case WM_MOUSEMOVE:
            {
                if (_bDownFlag)
                {
                    int2    mouseCur    =  int2(LOWORD(lParam),HIWORD(lParam));
                    float   xOffset     =  mouseCur.x - _mouseDown.x;
                    _mouseDown  =   mouseCur;
                    _camera[_cameraIndex].rotateY(xOffset * 0.5f);
                }
            }
            break;
        case WM_KEYDOWN:
            {
                switch (wParam)
                {
                case VK_LEFT:
                    _rolePos.x  -=  0.1;
                    break;
                case VK_RIGHT:
                    _rolePos.x  +=  0.1;
                    break;
                case  VK_UP:
                    _rolePos.z  -=  0.1;
                    break;
                case VK_DOWN:
                    _rolePos.z  +=  0.1;
                    break;

                }
            }
            break;
        }
        return  __super::events(hWnd,msgId,wParam,lParam);
    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{

    SamplerTexture    instance;
    instance.main(800,600);


    return  0;
}