
#include <windows.h>
#include <tchar.h>
#include <math.h>


#include "CELLMath.hpp"

#include "OpenGLWindow.h"
#include <vector>
using namespace CELL;

struct Vertex
{
    float x, y, z;
};
class   SamplerCube :public OpenGLWindow
{
public:
    SamplerCube() 
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

        std::vector<Vertex> arVertex;
        
        for (int i = 0 ;i < arVertex.size(); ++ i )
        {
            arVertex[i].z   -=  5;
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(Vertex),cubeVertices);
        glDrawArrays( GL_QUADS, 0, 24 );
    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerCube    instance;
    instance.main(800,600);
    return  0;
}