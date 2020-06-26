


#include "OpenGLWindow.h"
#include <mmsystem.h>
#include "freeimage.h"
#include "CELLMath.hpp"

using namespace CELL;

#pragma comment(lib,"winmm.lib")


struct Vertex
{
    float r, g, b;
    float x, y, z;
};

Vertex g_cubeVertices[] =
{
    { 1.0f,0.0f,0.0f, -1.0f,-1.0f, 1.0f }, // 0
    { 0.0f,1.0f,0.0f, 1.0f,-1.0f, 1.0f }, // 1
    { 0.0f,0.0f,1.0f, 1.0f, 1.0f, 1.0f }, // 2
    { 1.0f,1.0f,0.0f, -1.0f, 1.0f, 1.0f }, // 3
    { 1.0f,0.0f,1.0f, -1.0f,-1.0f,-1.0f }, // 4
    { 0.0f,1.0f,1.0f, -1.0f, 1.0f,-1.0f }, // 5
    { 1.0f,1.0f,1.0f, 1.0f, 1.0f,-1.0f }, // 6
    { 1.0f,0.0f,0.0f, 1.0f,-1.0f,-1.0f }, // 7
};


GLubyte g_cubeIndices[] =
{
    0, 1, 2, 3, // Quad 0
    4, 5, 6, 7, // Quad 1
    5, 3, 2, 6, // Quad 2
    4, 7, 1, 0, // Quad 3
    7, 6, 2, 1, // Quad 4
    4, 0, 3, 5 // Quad 5
};


class   IndexBufferObject :public OpenGLWindow
{
public:
    GLuint  _texture;
    GLuint  _vbo;
    GLuint  _ibo;
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

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, double(_width) / double(_height), 0.1f, 100.0f);

        glGenBuffers(1,&_vbo);
        glBindBuffer(GL_ARRAY_BUFFER,_vbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(g_cubeVertices),g_cubeVertices,GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);


        glGenBuffers(1,&_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(g_cubeIndices),g_cubeIndices,GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
      
    }
    virtual void    onShutdownGL()
    {
    }

    void    renderCube()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, double(_width) / double(_height), 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5);


        glBindBuffer(GL_ARRAY_BUFFER,_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibo);

        glEnableClientState(GL_VERTEX_ARRAY);

        glEnableClientState(GL_COLOR_ARRAY);


        /**
        * 这里大家可以慢慢体会
        */
        float* addrVertex = (float*)12;

        float* colorAddress = (float*)0;

        //--------------元素个数---元素类型---元素之间的内存偏移---数据地址
        //OpenGL根据元素之间的内存偏移来计算下一个元素的位置。
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), addrVertex);
        glColorPointer(3, GL_FLOAT, sizeof(Vertex), colorAddress);

        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, 0);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
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
    IndexBufferObject   context;

    context.main(640, 480);
    return 0;
}