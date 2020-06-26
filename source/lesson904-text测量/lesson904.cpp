
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "FreeImage.h"


#include "CELLMath.hpp"

#include "OpenGLWindow.h"
#include "Camera3rd.h"
#include "FreetypeFont.h"
#include <vector>
using namespace CELL;

struct Vertex
{ 
    float x, y, z;
    float u, v;
};


struct  UIObject
{
public:
    UIObject()
    {}

    virtual ~UIObject()
    {}
    float2  _pos;
    float2  _size;
};

struct  UIText :public UIObject
{
public:
    Rgba        _color;
    wchar_t     _text[32];
};

typedef std::vector<UIText> ArrayText;

struct VertexNormal
{ 
    float x, y, z;
    float nx,ny,nz;
};

struct GroundVertex
{ 
    float x, y, z;
    float u,v;
};


class   SamplerTexture :public OpenGLWindow
{
    GLuint  _texture;

    GLuint  _texGround;

    Camera3rd  _camera;
    int2    _mouseDown;
    bool    _bDownFlag;


    float3  _rolePos;
    float3  _moveTo;
    float   _angle;
    FreeTypeFont*_font;

    FreeTypeFont*_font43;

    ArrayText   _texts;
    bool        _bRButton;
    UIObject*   _pSelect;
public:
    SamplerTexture() 
    {
        _bDownFlag  =   false;
        _rolePos    =   float3(0,0,0);
        _moveTo     =   _rolePos;
        _angle      =   0;
        _pSelect    =   0;

        UIText  text1;
        text1._pos  =   float2(100,100);
        text1._color=   Rgba(255,0,0);
        wcscpy(text1._text,L"文本1");
        _texts.push_back(text1);


        UIText  text2;
        text2._pos  =   float2(100,200);
        text2._color=   Rgba(0,255,0);
        wcscpy(text2._text,L"abcdefg中1");

        _texts.push_back(text2);
    }         
    /**
    *   使用FreeImage加载图片
    */
    unsigned        createTextureFromImage(const char* fileName)
    {
        //1 获取图片格式
        FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);
        if (fifmt == FIF_UNKNOWN)
        {
            return  0;
        }
        //2 加载图片
        FIBITMAP    *dib = FreeImage_Load(fifmt, fileName,0);
     
        FREE_IMAGE_COLOR_TYPE type    =   FreeImage_GetColorType(dib);
      
        //! 获取数据指针
        FIBITMAP*   temp    =   dib;
        dib =   FreeImage_ConvertTo32Bits(dib);
                FreeImage_Unload(temp);

        BYTE*   pixels =   (BYTE*)FreeImage_GetBits(dib);
        int     width   =   FreeImage_GetWidth(dib);
        int     height  =   FreeImage_GetHeight(dib);

        for (int i = 0 ;i < width * height * 4 ; i+=4 )
        {
            BYTE temp       =   pixels[i];
            pixels[i]       =   pixels[i + 2];
            pixels[i + 2]   =   temp;
        }

        unsigned    res =   createTexture(width,height,pixels);
        FreeImage_Unload(dib);
        return      res;
    }

    unsigned        createTexture(int w,int h,const void* data)
    {
        unsigned    texId;
        glGenTextures(1,&texId);
        glBindTexture(GL_TEXTURE_2D,texId);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);

        return  texId;
    }

    
    virtual void    onInitGL()
    {
        _texture    =   createTextureFromImage("2.jpg");
        _texGround  =   createTextureFromImage("1.jpg");

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        GLfloat diffuse_light0[]    = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat position_light0[]   = { 0, 10, 0, 0.0f };
        glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light0 );
        glLightfv( GL_LIGHT0, GL_POSITION, position_light0 );

        _font   =   new FreeTypeFont();
        _font->create("simsun.ttc",16,512,512);

        _font43 =   new FreeTypeFont();

        _font43->create("simsun.ttc",40,512,512);

    }

    
    virtual void    renderNormalCube(const matrix4& mvp)
    {
        VertexNormal cubeVertices[] =
        {
            {+1.0,-1.0,+1.0,    1.0, 0.0, 0.0}, 
            {+1.0,-1.0,-1.0,    1.0, 0.0, 0.0},
            {+1.0,+1.0,-1.0,    1.0, 0.0, 0.0},
            {+1.0,+1.0,+1.0,    1.0, 0.0, 0.0},

            {+1.0,+1.0,+1.0,    0.0, 1.0, 0.0},  
            {+1.0,+1.0,-1.0,    0.0, 1.0, 0.0},  
            {-1.0,+1.0,-1.0,    0.0, 1.0, 0.0},  
            {-1.0,+1.0,+1.0,    0.0, 1.0, 0.0}, 

            {+1.0,+1.0,+1.0,    0.0, 0.0, 1.0},  
            {-1.0,+1.0,+1.0,    0.0, 0.0, 1.0}, 
            {-1.0,-1.0,+1.0,    0.0, 0.0, 1.0}, 
            {+1.0,-1.0,+1.0,    0.0, 0.0, 1.0}, 

            {-1.0,-1.0,+1.0,   -1.0, 0.0, 0.0}, 
            {-1.0,+1.0,+1.0,   -1.0, 0.0, 0.0},  
            {-1.0,+1.0,-1.0,   -1.0, 0.0, 0.0},  
            {-1.0,-1.0,-1.0,   -1.0, 0.0, 0.0}, 

            {-1.0,-1.0,+1.0,    0.0,-1.0, 0.0},  
            {-1.0,-1.0,-1.0,    0.0,-1.0, 0.0},  
            {+1.0,-1.0,-1.0,    0.0,-1.0, 0.0},  
            {+1.0,-1.0,+1.0,    0.0,-1.0, 0.0}, 

            {-1.0,-1.0,-1.0,    0.0, 0.0,-1.0},  
            {-1.0,+1.0,-1.0,    0.0, 0.0,-1.0},  
            {+1.0,+1.0,-1.0,    0.0, 0.0,-1.0},  
            {+1.0,-1.0,-1.0,    0.0, 0.0,-1.0}, 
        };

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(mvp.data());

        glRotatef(_angle,1,1,1);
        _angle  +=  0.1f;
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3,GL_FLOAT,         sizeof(VertexNormal),     &cubeVertices[0].x);
        glNormalPointer(GL_FLOAT,           sizeof(VertexNormal),     &cubeVertices[0].nx);

        glDrawArrays( GL_QUADS, 0, 24 );
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisable(GL_LIGHTING);

    }


    
    virtual void    renderRole(const matrix4& mvp)
    {
        Vertex cubeVertices[] =
        {
            {  -1.0f,-1.0f, 1.0f    ,0.3f,  0.3f },
            {  1.0f,-1.0f, 1.0f     ,0.5f,  0.3f },
            {  1.0f, 1.0f, 1.0f     ,0.5f,  0.8f },
            { -1.0f, 1.0f, 1.0f     ,0.3f,  0.8f },

            {  -1.0f,-1.0f,-1.0f    ,0,  0 },
            {  -1.0f, 1.0f,-1.0f    ,1,  0 },
            {  1.0f, 1.0f,-1.0f     ,1,  1 },
            {  1.0f,-1.0f,-1.0f     ,0,  1 },

            {  -1.0f, 1.0f,-1.0f    ,0,  0 },
            {  -1.0f, 1.0f, 1.0f    ,1,  0 },
            {  1.0f, 1.0f, 1.0f     ,1,  1 },
            {  1.0f, 1.0f,-1.0f     ,0,  1 },

            {  -1.0f,-1.0f,-1.0f    ,0,  0 },
            {  1.0f,-1.0f,-1.0f     ,1,  0 },
            {  1.0f,-1.0f, 1.0f     ,1,  1 },
            {  -1.0f,-1.0f, 1.0f    ,0,  1 },

            {  1.0f,-1.0f,-1.0f ,   0,  0 },
            {  1.0f, 1.0f,-1.0f ,   1,  0 },
            {  1.0f, 1.0f, 1.0f ,   1,  1 },
            {  1.0f,-1.0f, 1.0f ,   0,  1 },

            {  -1.0f,-1.0f,-1.0f ,  0,  0 },
            {  -1.0f,-1.0f, 1.0f ,  1,  0 },
            {  -1.0f, 1.0f, 1.0f ,  1,  1 },
            {  -1.0f, 1.0f,-1.0f ,  0,  1 },
        };

        glLoadMatrixf(mvp.data());

        //! 产生一个矩阵
        glTranslatef(_rolePos.x,_rolePos.y,_rolePos.z);

        glBindTexture(GL_TEXTURE_2D,_texture);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            glVertexPointer(3,GL_FLOAT,         sizeof(Vertex),     &cubeVertices[0].x);
            glTexCoordPointer(2,GL_FLOAT,       sizeof(Vertex),     &cubeVertices[0].u);

        glDrawArrays( GL_QUADS, 0, 24 );
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }


    
    virtual void    renderGround(const matrix4& mvp)
    {
        float   size    =   100;

        Vertex  ground[] =
        {
            {  -size, -1,   -size       ,0,  0  },
            {  -size, -1,   size        ,100,0  },
            {  size,  -1,   size        ,100,100},
            {  size,  -1,   -size       ,0,  100},
        };

        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);


        glBindTexture(GL_TEXTURE_2D,_texGround);

        glLoadMatrixf(mvp.data());

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3,GL_FLOAT,         sizeof(Vertex),     &ground[0].x);
        glTexCoordPointer(2,GL_FLOAT,       sizeof(Vertex),     &ground[0].u);


        glDrawArrays(GL_QUADS,0,4);
    }
    

    virtual void    renderGUI()
    {

        _font->begin(_width,_height);

        for (ArrayText::iterator itr = _texts.begin(); itr != _texts.end() ; ++ itr )
        {
            (*itr)._size    =   _font->drawText((*itr)._pos.x,(*itr)._pos.y,0,(*itr)._color,(*itr)._text,0,0,0);
        }

        _font->end();
       
    }

    virtual void    render()
    {

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glMatrixMode(GL_MODELVIEW);

        _camera.perspective(60,float(_width)/float(_height),0.1f,1000.0f);

        float3  dir =   normalize(_moveTo - _rolePos);

        if (length(_moveTo - _rolePos) < 1)
        {
            _rolePos    =   _moveTo;
        }
        else
        {
            _rolePos    +=  dir * (10.0f/60.0f);
        }

        _camera.updateLookat(_rolePos);
        _camera.update();

        matrix4 matVP  =   _camera.getMVP();

        renderNormalCube(matVP);
        renderRole(matVP);
        renderGround(matVP);

        renderGUI();
    }


    
    
    virtual LRESULT events(HWND hWnd,UINT msgId, WPARAM wParam, LPARAM lParam)
    {
        switch (msgId)
        {
        case WM_SIZE:
            {
                if (::IsWindow(hWnd))
                {
                    RECT    rt;
                    GetClientRect(_hWnd,&rt);
                    int     w   =   rt.right - rt.left;
                    int     h   =   rt.bottom - rt.top;
                    glViewport(0,0,w ,h );
                    _camera.setView(w,h);
                }
            }
            break;
        case WM_LBUTTONDOWN:
            {
                _bDownFlag  =   true;
                _mouseDown  =   int2(LOWORD(lParam),HIWORD(lParam));

                CELL::Ray   ray     =   _camera.createRayFromScreen(_mouseDown.x,_mouseDown.y);
                float3      orth    =   ray.getOrigin();
                float       tm      =   (orth.y  + 1)/ray.getDirection().y;
                float3      c       =   ray.getOrigin() + abs(tm) * ray.getDirection();
                _moveTo =   c;
            }
            break;
        case WM_LBUTTONUP:
            {
                _bDownFlag  =   false;
            }
            break;

        case WM_RBUTTONDOWN:
            {
                _mouseDown  =   int2(LOWORD(lParam),HIWORD(lParam));
                _bRButton   =   true;
                _pSelect    =   0;

                ArrayText::iterator itr = _texts.begin();

                for ( ; itr != _texts.end(); ++ itr )
                {
                    trect<float>    rt((*itr)._pos.x,(*itr)._pos.y,(*itr)._pos.x + (*itr)._size.x,(*itr)._pos.y + (*itr)._size.y);
                    if (rt.ptInRect(_mouseDown.x,_mouseDown.y))
                    {
                        _pSelect    =   &(*itr);
                        break;
                    }
                }
            }
            break;

        case WM_RBUTTONUP:
            {
                _bRButton   =   false;
                _pSelect    =   0;
            }

        case WM_MOUSEWHEEL:
            {
                int delta   =   (short)HIWORD(wParam);
                if (delta > 0)
                {
                    _camera.setRadius(_camera.getRadius() * 1.2f);
                }
                else
                {
                    _camera.setRadius(_camera.getRadius() * 0.8f);
                }
            }
            break;

        case WM_MOUSEMOVE:
            {
                if (_bDownFlag)
                {
                    int2    mouseCur    =  int2(LOWORD(lParam),HIWORD(lParam));
                    float   xOffset     =  mouseCur.x - _mouseDown.x;
                    _mouseDown  =   mouseCur;
                    _camera.rotateY(xOffset * 0.5f);
                }
                else if(_bRButton && _pSelect)
                {
                    int2    mouseCur    =  int2(LOWORD(lParam),HIWORD(lParam));
                    float   xOffset     =  mouseCur.x - _mouseDown.x;
                    float   yOffset     =  mouseCur.y - _mouseDown.y;

                    _mouseDown  =   mouseCur;

                    _pSelect->_pos  +=  float2(xOffset,yOffset);

                }
            }
            break;
        case WM_KEYDOWN:
            {
                switch (wParam)
                {
                case VK_LEFT:
                    _rolePos.x  -=  0.1;
                    break;
                case VK_RIGHT:
                    _rolePos.x  +=  0.1;
                    break;
                case  VK_UP:
                    _rolePos.z  -=  0.1;
                    break;
                case VK_DOWN:
                    _rolePos.z  +=  0.1;
                    break;

                }
            }
            break;
        }
        return  __super::events(hWnd,msgId,wParam,lParam);
    }
};


int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{

    SamplerTexture    instance;
    instance.main(800,600);


    return  0;
}