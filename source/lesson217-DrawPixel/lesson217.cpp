
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "OpenGLWindow.h"


class   SamplerDrawPixel :public OpenGLWindow
{
public:

    struct  Vertex
    {
        float   x,y,z;
        float   r,g,b;
    };

    char*   _pixel;

    SamplerDrawPixel()
    {
        _pixel  =   new char[100 * 200 * 4];
        for (int i = 0 ; i < 100 * 200 * 4 ; ++ i)
        {
            _pixel[i]   =   rand()%255;
        }
    }
    virtual void    render()
    {
        #define M_PI (3.14159265358979323846)
        //! ָ�����µĲ������ͶӰ����
        glMatrixMode(GL_PROJECTION);
        //! ��ͶӰ��֤��ճɵ�λ����
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);


        glColor3f(1,0,1);

        Vertex  rect[]  =   
        {
            {10,    10,     0,  1,  0,  0},
            {110,   10,     0,  0,  1,  0},
            {10,    110,    0,  0,  0,  1},
            {110,   110,    0,  1,  0,  1},
        };
        glColor3f(1,0,1);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(Vertex),rect);
        glColorPointer(3,GL_FLOAT,sizeof(Vertex),&rect[0].r);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);


        Vertex  rect1[]  =   
        {
            {10,    10,     0,  1,  0,  0},
            {110,   10,     0,  0,  1,  0},
            {10,    110,    0,  0,  0,  1},
            {110,   110,    0,  1,  0,  1},
        };

        for (int i = 0 ;i < 4 ; ++ i )
        {
            rect1[i].x += 100;
        }

        glVertexPointer(3,GL_FLOAT,sizeof(Vertex),rect1);
        glColorPointer(3,GL_FLOAT,sizeof(Vertex),&rect1[0].r);
        glColor3f(0,0,1);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        glRasterPos2i(100,300);
        glDrawPixels(100,200,GL_RGBA,GL_UNSIGNED_BYTE,_pixel);

    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerDrawPixel    instance;
    instance.main(800,600);


    return  0;
}