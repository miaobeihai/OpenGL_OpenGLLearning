
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "OpenGLWindow.h"


class   SamplerTriangle :public OpenGLWindow
{
public:

    struct  float3
    {
        float   x,y,z;
        float   w;
    };

    float3  _circle[360 * 3];

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

        for (int i = 0 ;i < 360 ; ++ i)
        {
            _circle[i * 3 + 0].x    =  cx; 
            _circle[i * 3 + 0].y    =  cy;
            _circle[i * 3 + 0].z    =  cz;

            _circle[i * 3 + 1].x    =  (float)cos((double)i * M_PI/180) * r + cx;; 
            _circle[i * 3 + 1].y    =  (float)sin((double)i * M_PI/180) * r+ cy;;
            _circle[i * 3 + 1].z    =  cz;

            _circle[i * 3 + 2].x    =  (float)cos((double)(i + 1) * M_PI/180) * r + cx; 
            _circle[i * 3 + 2].y    =  (float)sin((double)(i + 1) * M_PI/180) * r + cy;
            _circle[i * 3 + 2].z    =  cz;
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(float3),_circle);
        glDrawArrays(GL_TRIANGLES,0,360 * 3);

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