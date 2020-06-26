
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
    };

    Vertex  _line[360];


    virtual void    render()
    {
        #define M_PI (3.14159265358979323846)
        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);


        Vertex  p0  =   {10,10,0};
        Vertex  p1  =   {100,100,0};
        Vertex  p2  =   {200,10,0};
        Vertex  p3  =   {300,150,0};

        int     indx    =   0;
        for (float t = 0 ; t < 1.0f; t += 0.01f, ++ indx)
        {
            _line[indx].x   =   (1 - t) * (1 - t) * (1 - t) * p0.x + 3 * t * (1 - t) * (1 - t)* p1.x + 3 * t*t* (1 -t)* p2.x + t * t * t * p3.x;
            _line[indx].y   =   (1 - t) * (1 - t) * (1 - t) * p0.y + 3 * t * (1 - t) * (1 - t)* p1.y + 3 * t*t* (1 -t)* p2.y + t * t * t * p3.y;
            _line[indx].z   =   0;
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(Vertex),_line);
        glDrawArrays(GL_LINE_STRIP,0, indx - 1);
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