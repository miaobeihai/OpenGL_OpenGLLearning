
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "CELLMath.hpp"

#include "OpenGLWindow.h"
#include <vector>
using namespace CELL;

class   SamplerSpline :public OpenGLWindow
{
public:

    tspline<float> _spline;

    SamplerSpline() 
    {
        _spline.addPoint(float3(10,10,0));
        _spline.addPoint(float3(20,100,0));
        _spline.addPoint(float3(100,80,0));
        _spline.addPoint(float3(200,100,0));
        _spline.addPoint(float3(300,10,0));
        _spline.addPoint(float3(400,150,0));

        for (float t = 0 ; t < 1.0f; t += 0.01f)
        {
            float3  pos =   _spline.interpolate(t);
            _array.push_back(pos);

        }
    }

    std::vector<float3> _array;

    virtual void    render()
    {
        #define M_PI (3.14159265358979323846)
        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);
       

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(float3),&_array[0]);
        glDrawArrays(GL_LINE_STRIP,0, _array.size());
    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerSpline    instance;
    instance.main(800,600);


    return  0;
}