
#include <windows.h>
#include <tchar.h>
#include <math.h>


#include "CELLMath.hpp"

#include "OpenGLWindow.h"
#include <vector>
using namespace CELL;

class   SamplerPerspective :public OpenGLWindow
{
public:
    SamplerPerspective() 
    {
    }

    virtual void    render()
    {
        #define M_PI (3.14159265358979323846)
        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();

        gluPerspective(60,double(_width)/double(_height),0.1,1000);
       
        float3  arVert[]    =   
        {
            float3(0.5f,0,-0.5),
            float3(0.2f,0.3,-.5),
            float3(0.8f,0.3,-.5),
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(float3),arVert);
        glDrawArrays(GL_TRIANGLES,0,3);
    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerPerspective    instance;
    instance.main(800,600);


    return  0;
}