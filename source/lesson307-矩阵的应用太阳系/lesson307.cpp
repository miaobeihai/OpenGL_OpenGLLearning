
#include <windows.h>
#include <tchar.h>
#include <math.h>


#include "CELLMath.hpp"

#include "OpenGLWindow.h"
#include <vector>
using namespace CELL;

struct Vertex
{
    unsigned char r,g,b,a;
    float x, y, z;
};
class   SamplerMatrix :public OpenGLWindow
{
public:

    float   _angle;
    float   _angleMoon;
    float   _angleEarth;
public:
    SamplerMatrix() 
    {
        _angle      =   0;
        _angleMoon  =   0;
        _angleEarth =   0;
    }

    virtual void    render()
    {
#define M_PI (3.14159265358979323846)
        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();

        gluPerspective(60,double(_width)/double(_height),0.1,1000);

        Vertex cubeVertices[] =
        {
            { 255,0,0,255, -1.0f,-1.0f, 1.0f },
            { 255,0,0,255, 1.0f,-1.0f, 1.0f },
            { 255,0,0,255, 1.0f, 1.0f, 1.0f },
            { 255,0,0,255, -1.0f, 1.0f, 1.0f },

            { 0,255,0,255, -1.0f,-1.0f,-1.0f },
            { 0,255,0,255, -1.0f, 1.0f,-1.0f },
            { 0,255,0,255, 1.0f, 1.0f,-1.0f },
            { 0,255,0,255, 1.0f,-1.0f,-1.0f },

            { 0,0,255,255, -1.0f, 1.0f,-1.0f },
            { 0,0,255,255, -1.0f, 1.0f, 1.0f },
            { 0,0,255,255, 1.0f, 1.0f, 1.0f },
            { 0,0,255,255, 1.0f, 1.0f,-1.0f },

            { 0,255,255,255, -1.0f,-1.0f,-1.0f },
            { 0,255,255,255, 1.0f,-1.0f,-1.0f },
            { 0,255,255,255, 1.0f,-1.0f, 1.0f },
            { 0,255,255,255, -1.0f,-1.0f, 1.0f },

            { 255,0,255,255, 1.0f,-1.0f,-1.0f },
            { 255,0,255,255, 1.0f, 1.0f,-1.0f },
            { 255,0,255,255, 1.0f, 1.0f, 1.0f },
            { 255,0,255,255, 1.0f,-1.0f, 1.0f },

            { 255,255,255,255, -1.0f,-1.0f,-1.0f },
            { 255,255,255,255, -1.0f,-1.0f, 1.0f },
            { 255,255,255,255, -1.0f, 1.0f, 1.0f },
            { 255,255,255,255, -1.0f, 1.0f,-1.0f }
        };
  
        glMatrixMode(GL_MODELVIEW);
#if 0
        glEnable(GL_DEPTH_TEST);
        glInterleavedArrays( GL_C4UB_V3F, 0, cubeVertices );
#else

        glEnable(GL_DEPTH_TEST);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3,GL_FLOAT,         sizeof(Vertex),     &cubeVertices[0].x);
        glColorPointer(4,GL_UNSIGNED_BYTE,  sizeof(Vertex),     cubeVertices);
#endif
        _angle  +=  1;
        //! 绘制太阳
        glLoadIdentity();
        glTranslatef(0,-10,-150);
        glScalef(2,2,2);
        glRotatef(_angle,0,1,0);
        
        glDrawArrays( GL_QUADS, 0, 24 );

        
        //! 绘制地球
        _angleEarth +=  3;
        glRotatef(_angleEarth,0,1,0);
        glTranslatef(0,0,-50);
        glScalef(0.5,0.5,0.5);
        glRotatef(_angle,0,1,0);
        glDrawArrays( GL_QUADS, 0, 24 );

        _angleMoon  +=  10;
        
        //! 绘制月亮
        glRotatef(_angleMoon,0,1,0);
        glTranslatef(0,0,-10);
        glScalef(0.5,0.5,0.5);
        glRotatef(_angle,0,1,0);
        glDrawArrays( GL_QUADS, 0, 24 );
        
    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{

    SamplerMatrix    instance;
    instance.main(800,600);


    return  0;
}