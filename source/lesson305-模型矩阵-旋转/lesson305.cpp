
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
class   SamplerRotate :public OpenGLWindow
{
public:

    float   _angle;
public:
    SamplerRotate() 
    {
        _angle  =   0;
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

        //         for (int i = 0 ;i < sizeof(cubeVertices)/sizeof(cubeVertices[0]); ++ i)
        //         {
        //             cubeVertices[i].x   *=  3;
        //             cubeVertices[i].y   *=  3;
        //             cubeVertices[i].z   *=  3;
        //         }
        //! 
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
        //! 清成单位矩阵
        glLoadIdentity();

        //C*M

        //! 产生一个矩阵
        glTranslatef(0,0,-5);
        //C*M*N
        //! 也产生矩阵
        glRotatef(_angle,1,0,0);
        glDrawArrays( GL_QUADS, 0, 24 );

        //! 清成单位矩阵
        glLoadIdentity();
        //! 产生一个矩阵 * 当前矩阵
        glTranslatef(3,0,-10);
        glRotatef(_angle,0,1,0);
        glDrawArrays( GL_QUADS, 0, 24 );

        //! 清成单位矩阵
        glLoadIdentity();
        glTranslatef(3,-3,-10);
        glRotatef(_angle,0,0,1);
        //! 产生一个矩阵 * 当前矩阵

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

    SamplerRotate    instance;
    instance.main(800,600);


    return  0;
}