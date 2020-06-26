
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "FreeImage.h"


#include "CELLMath.hpp"

#include "OpenGLWindow.h"
#include "Camera3rd.h"
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

    Camera3rd  _camera;
    int2    _mouseDown;
    bool    _bDownFlag;


    float3  _rolePos;
public:
    SamplerTexture() 
    {
        _bDownFlag  =   false;
        _rolePos    =   float3(0,0,0);
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
        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();

        gluPerspective(60,double(_width)/double(_height),0.1,1000);

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
            {  size, -1,     size        ,100,  100 },
            {  size, 0,     -size       ,0,  100 },
        };

        

        glMatrixMode(GL_MODELVIEW);
        //! 清成单位矩阵
        glLoadIdentity();

        _camera.updateLookat(_rolePos);
        //! 摄像机更新
        _camera.update();

        glPushMatrix();
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

        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D,_texGround);
        glDrawArrays(GL_QUADS,24,4);


    }

    virtual LRESULT events(HWND hWnd,UINT msgId, WPARAM wParam, LPARAM lParam)
    {
        switch (msgId)
        {
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
        case WM_MOUSEMOVE:
            {
                if (_bDownFlag)
                {
                    int2    mouseCur    =  int2(LOWORD(lParam),HIWORD(lParam));
                    float   xOffset     =  mouseCur.x - _mouseDown.x;
                    _mouseDown  =   mouseCur;
                    _camera.rotateY(xOffset * 0.5f);
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