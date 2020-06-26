
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "OpenGLWindow.h"


class   SamplerQTrip :public OpenGLWindow
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
        //! ָ�����µĲ������ͶӰ����
        glMatrixMode(GL_PROJECTION);
        //! ��ͶӰ��֤��ճɵ�λ����
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);


        Vertex  vertex[]    =   
        {
            {10,110,0},
            {10,10,0},

            {110,110,0},
            {110,10,0},

            {210,110,0},
            {210,10,0},
        };
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(Vertex),vertex);
        glDrawArrays(GL_TRIANGLE_STRIP,0, sizeof(vertex)/sizeof(vertex[0]));
    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerQTrip    instance;
    instance.main(800,600);


    return  0;
}