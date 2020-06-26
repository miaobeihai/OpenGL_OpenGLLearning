
#include <windows.h>
#include <tchar.h>

#include "OpenGLWindow.h"


class   SamplerTriangle :public OpenGLWindow
{
public:


    virtual void    render()
    {
        //! ָ�����µĲ������ͶӰ����
        glMatrixMode(GL_PROJECTION);
        //! ��ͶӰ��֤��ճɵ�λ����
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);

        glColor3f(1,1,1);
        glBegin(GL_TRIANGLES);

            glVertex3f(_width * 0.5f,0,0);
            glVertex3f(_width,_height,0);
            glVertex3f(0,_height,0);

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
   
    OpenGLWindow    instance(hInstance);
    instance.main(800,600);


    return  0;
}