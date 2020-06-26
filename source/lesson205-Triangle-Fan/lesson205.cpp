
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "OpenGLWindow.h"


class   SamplerTriangleFan :public OpenGLWindow
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

        float   cx  =   100;
        float   cy  =   100;
        float   cz  =   0;
        float   r   =   80;

        glColor3f(1,1,1);

        _circle[0].x    =  cx; 
        _circle[0].y    =  cy;
        _circle[0].z    =  cz;

        for (int i = 0 ;i <= 360 ; ++ i)
        {
            _circle[i + 1].x    =  (float)cos((double)i * M_PI/180) * r + cx; 
            _circle[i + 1].y    =  (float)sin((double)i * M_PI/180) * r + cy;
            _circle[i + 1].z    =  cz;
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(float3),_circle);
        glDrawArrays(GL_TRIANGLE_FAN,0,362);

    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerTriangleFan    instance;
    instance.main(800,600);


    return  0;
}