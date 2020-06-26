
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "OpenGLWindow.h"


class   SamplerTriangleStrip :public OpenGLWindow
{
public:

    struct  float3
    {
        float   x,y,z;
    };

    float3  _circle[362];

    virtual void    render()
    {
        #define M_PI (3.14159265358979323846)
        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);


        glColor3f(1,1,1);

        float3  rect[]  =   
        {
            {10,    10,     0},
            {110,   10,     0},
            {10,    110,    0},
            {110,   110,    0},
        };
       
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(float3),rect);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerTriangleStrip    instance;
    instance.main(800,600);


    return  0;
}