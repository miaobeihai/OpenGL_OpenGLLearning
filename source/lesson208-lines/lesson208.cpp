
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "OpenGLWindow.h"


class   SamplerLine :public OpenGLWindow
{
public:

    struct  Vertex
    {
        float   x,y,z;
        float   r,g,b;
    };

    virtual void    render()
    {
        #define M_PI (3.14159265358979323846)
        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);


        glColor3f(1,0,1);

        Vertex  rect[]  =   
        {
            {10,    10,     0,  1,  0,  0},
            {110,   10,     0,  0,  1,  0},
            {160,   30,    0,  0,  0,  1},
            {200,   110,    0,  1,  0,  1},
            {160,   180,    0,  1,  0,  1},
        };
        glColor3f(1,0,1);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(Vertex),rect);
        glColorPointer(3,GL_FLOAT,sizeof(Vertex),&rect[0].r);
        glDrawArrays(GL_LINE_LOOP,1,4);
    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerLine    instance;
    instance.main(800,600);


    return  0;
}