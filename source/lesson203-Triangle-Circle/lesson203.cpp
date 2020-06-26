
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "OpenGLWindow.h"


class   SamplerTriangle :public OpenGLWindow
{
public:


    virtual void    render()
    {
        #define M_PI (3.14159265358979323846)
        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);

        float   cx  =   100;
        float   cy  =   100;
        float   cz  =   0;
        float   r   =   80;

        glColor3f(1,1,1);

        glBegin(GL_TRIANGLES);

        for (int i = 0 ;i < 360 ; ++ i)
        {
            float x     =  (float)cos((double)i * M_PI/180) * r + cx;
            float y     =  (float)sin((double)i * M_PI/180) * r+ cy;

            float x1    =  (float)cos((double)(i + 1) * M_PI/180) * r + cx;
            float y1    =  (float)sin((double)(i + 1) * M_PI/180) * r+ cy;
            glVertex3f(cx,cy,cz);
            glVertex3f(x,y,cz);
            glVertex3f(x1,y1,cz);
            
        }
        glEnd();

        
        
    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerTriangle    instance;
    instance.main(800,600);


    return  0;
}