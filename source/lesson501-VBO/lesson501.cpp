


#include "OpenGLWindow.h"
#include <mmsystem.h>
#include "freeimage.h"
#include "CELLMath.hpp"

using namespace CELL;

#pragma comment(lib,"winmm.lib")


struct Vertex
{
    float x, y, z;
    float u, v;
    float r, g, b;
};


Vertex g_cubeVertices[] =
{
    { -1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 0.0f, 0.0f },
    { 1.0f,-1.0f, 1.0f,1.0f, 0.0f,1.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f,1.0f, 1.0f,1.0f, 0.0f, 0.0f },
    { -1.0f, 1.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f, 0.0f },

    { -1.0f,-1.0f,-1.0f,1.0f, 0.0f,0.0f, 1.0f, 0.0f },
    { -1.0f, 1.0f,-1.0f,1.0f, 1.0f,0.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f,-1.0f,0.0f, 1.0f,0.0f, 1.0f, 0.0f },
    { 1.0f,-1.0f,-1.0f,0.0f, 0.0f,0.0f, 1.0f, 0.0f },

    { -1.0f, 1.0f,-1.0f,0.0f, 1.0f,0.0f, 0.0f, 1.0f },
    { -1.0f, 1.0f, 1.0f,0.0f, 0.0f,0.0f, 0.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f,1.0f, 0.0f,0.0f, 0.0f, 1.0f },
    { 1.0f, 1.0f,-1.0f,1.0f, 1.0f,0.0f, 0.0f, 1.0f },

    { -1.0f,-1.0f,-1.0f,1.0f, 1.0f,1.0f, 1.0f, 0.0f },
    { 1.0f,-1.0f,-1.0f,0.0f, 1.0f,1.0f, 1.0f, 0.0f },
    { 1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 1.0f, 0.0f },
    { -1.0f,-1.0f, 1.0f,1.0f, 0.0f,1.0f, 1.0f, 0.0f },

    { 1.0f,-1.0f,-1.0f,1.0f, 0.0f,1.0f, 0.0f, 1.0f },
    { 1.0f, 1.0f,-1.0f,1.0f, 1.0f,1.0f, 0.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f,0.0f, 1.0f,1.0f, 0.0f, 1.0f },
    { 1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 0.0f, 1.0f },

    { -1.0f,-1.0f,-1.0f,0.0f, 0.0f,0.0f, 1.0f, 1.0f },
    { -1.0f,-1.0f, 1.0f,1.0f, 0.0f,0.0f, 1.0f, 1.0f },
    { -1.0f, 1.0f, 1.0f,1.0f, 1.0f,0.0f, 1.0f, 1.0f },
    { -1.0f, 1.0f,-1.0f,0.0f, 1.0f,0.0f, 1.0f, 1.0f }
};



class   VertexBufferObject :public OpenGLWindow
{
public:
    GLuint  _texture;
    GLuint  _vbo;
public:
    unsigned    createTexture(int w, int h, const void* data, GLenum type)
    {
        unsigned    texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, type, GL_UNSIGNED_BYTE, data);


        return  texId;
    }
    /**
    *   使用FreeImage加载图片
    */
    unsigned    createTextureFromImage(const char* fileName)
    {
        //1 获取图片格式
        FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
        if (fifmt == FIF_UNKNOWN)
        {
            return  0;
        }
        //2 加载图片
        FIBITMAP    *dib = FreeImage_Load(fifmt, fileName, 0);

        FREE_IMAGE_COLOR_TYPE type = FreeImage_GetColorType(dib);

        //! 获取数据指针
        FIBITMAP*   temp = dib;
        dib = FreeImage_ConvertTo32Bits(dib);
        FreeImage_Unload(temp);

        BYTE*   pixels = (BYTE*)FreeImage_GetBits(dib);
        int     width = FreeImage_GetWidth(dib);
        int     height = FreeImage_GetHeight(dib);

        for (int i = 0; i < width * height * 4; i += 4)
        {
            BYTE temp = pixels[i];
            pixels[i] = pixels[i + 2];
            pixels[i + 2] = temp;
        }

        unsigned    res = createTexture(width, height, pixels, GL_RGBA);
        FreeImage_Unload(dib);
        return      res;
    }
public:
    virtual void    onInitGL()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, double(_width) / double(_height), 0.1f, 100.0f);

        _texture    =   createTextureFromImage("floor.bmp");


        glGenBuffers(1,&_vbo);
        glBindBuffer(GL_ARRAY_BUFFER,_vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(g_cubeVertices),g_cubeVertices,GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER,0);

      
    }
    virtual void    onShutdownGL()
    {
        glDeleteBuffers(1,&_vbo);
    }

    void    renderCube()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, double(_width) / double(_height), 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5);
        glBindTexture(GL_TEXTURE_2D, _texture);

        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER,_vbo);

        //glEnable(GL_DEPTH_TEST);
        /**
        * 这里大家可以慢慢体会
        */
        float* addrVertex = (float*)0;
        float* uvAddress = (float*)12;

        float* colorAddress = (float*)20;

        //--------------元素个数---元素类型---元素之间的内存偏移---数据地址
        //OpenGL根据元素之间的内存偏移来计算下一个元素的位置。
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), addrVertex);
        glColorPointer(3, GL_FLOAT, sizeof(Vertex), colorAddress);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), uvAddress);

        glDrawArrays(GL_QUADS, 0, 24);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    virtual void render(void)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //! 立方体
       renderCube();       
    }
};



int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    VertexBufferObject   context;

    context.main(640, 480);
    return 0;
}