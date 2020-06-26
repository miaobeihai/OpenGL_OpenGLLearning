
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
    Rgba    _color;
public:
    virtual void    mouseEnter(int2 pos)
    {
    }
    virtual void    mouseLeave(int2 pos)
    {
    }

    virtual void    mouseClick(int2 pos)
    {
    }

    virtual void    render()
    {}
};

typedef void (*LPDWON)(UIObject*);

struct  UIText :public UIObject
{
public:
    wchar_t     _text[32];
    LPDWON      _eventDown;
    LPDWON      _eventClick;

};
class   UIButton :public UIObject
{
public:
    struct  Vertex
    {
        float x,y,z;
        Rgba  color;
    };
public:
    enum
    {
        STATUS_NORMAL,
        STATUS_HOVER,
        STATUS_PUSH,
        STATUS_DISABLE,
        STATUS_MAX,
    };

    int     _status;

    Rgba    _statusColor[STATUS_MAX];
public:
    UIButton()
    {
        _status     =   STATUS_NORMAL;
        _statusColor[STATUS_NORMAL] =   Rgba(180,180,180,100);
        _statusColor[STATUS_HOVER]  =   Rgba(100,255,255,255);
        _statusColor[STATUS_PUSH]   =   Rgba(255,255,255,255);
        _statusColor[STATUS_DISABLE]=   Rgba(80,80,80,255);
    }

    virtual void    mouseEnter(int2 pos)
    {
        _status     =   STATUS_HOVER;
        OutputDebugStringA("mouseEnter\n");
    }
    virtual void    mouseLeave(int2 pos)
    {
        _status     =   STATUS_NORMAL;
        OutputDebugStringA("mouseLeave\n");
    }

    virtual void    mouseClick(int2 pos)
    {
        _status     =   STATUS_PUSH;
        OutputDebugStringA("mouseClick\n");
    }
    virtual void    render()
    {
        Vertex  vertex[]    =   
        {
            {_pos.x,            _pos.y,0,           _statusColor[_status]},
            {_pos.x + _size.x,  _pos.y,0,           _statusColor[_status]},
            {_pos.x + _size.x,  _pos.y + _size.y,0, _statusColor[_status]},
            {_pos.x,            _pos.y + _size.y,0, _statusColor[_status]},
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(3,GL_FLOAT,         sizeof(Vertex),     &vertex[0].x);
        glColorPointer(4,GL_UNSIGNED_BYTE,  sizeof(Vertex),     &vertex[0].color);

        glDrawArrays( GL_QUADS, 0, 4 );

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
};


class   UIManager :public std::vector<UIObject*>
{
protected:
    UIObject*   _mouseIn;
    UIObject*   _mouseDown;
public:
    UIManager()
    {
        _mouseIn    =   0;
        _mouseDown  =   0;
    }

    void    onMouseEnter(UIObject* obj,int2 pos)
    {
        obj->mouseEnter(pos);
    }

    void    onMouseLeave(UIObject* obj,int2 pos)
    {
        obj->mouseLeave(pos);
    }


    void    onMouseClick(UIObject* obj,int2 pos)
    {
        obj->mouseClick(pos);
    }
    void    onMessage(int msgId,int2 mousPos)
    {
        switch(msgId)
        {
        case WM_MOUSEMOVE:
            {
                UIObject* mouseEnterObj    =   0;
                for (iterator itr = begin() ; itr != end(); ++ itr )
                {
                    UIObject*   obj =   (*itr);
                    float2      pos =   obj->_pos;
                    float2      sz  =   obj->_size;

                    trect<float>    rect(pos.x,pos.y,pos.x + sz.x,pos.y + sz.y);
                    if (rect.ptInRect(mousPos.x,mousPos.y))
                    {
                        mouseEnterObj  =   obj;
                        if (_mouseIn == 0)
                        {
                            _mouseIn    =   obj;
                            onMouseEnter(obj,pos);
                        }
                        else if(_mouseIn != obj)
                        {
                            onMouseLeave(_mouseIn,pos);
                            _mouseIn    =   obj;
                            onMouseEnter(obj,mousPos);
                        }
                        else
                        {
                        }
                        break;
                    }
                }
                if (mouseEnterObj == 0)
                {
                    if (_mouseIn != 0)
                    {
                        onMouseLeave(_mouseIn,mousPos);
                        _mouseIn    =   0;
                    }
                }
            }
            break;
        case WM_LBUTTONDOWN:
            {
                for (iterator itr = begin() ; itr != end(); ++ itr )
                {
                    UIObject*   obj =   (*itr);
                    float2  pos =   obj->_pos;
                    float2  sz  =   obj->_size;

                    trect<float>    rect(pos.x,pos.y,pos.x + sz.x,pos.y + sz.y);
                    if (rect.ptInRect(mousPos.x,mousPos.y))
                    {
                        _mouseDown  =   obj;
                        break;
                    }
                }
            }
            break;
        case WM_LBUTTONUP:
            {
                if (_mouseDown)
                {
                    float2  pos =   _mouseDown->_pos;
                    float2  sz  =   _mouseDown->_size;

                    trect<float>    rect(pos.x,pos.y,pos.x + sz.x,pos.y + sz.y);
                    if (rect.ptInRect(mousPos.x,mousPos.y))
                    {
                        onMouseClick(_mouseDown,mousPos);
                    }
                }
                else
                {

                }
            }
            break;
        }
    }


    void    render()
    {
        for ( iterator itr = begin() ; itr != end() ; ++ itr )
        {
            UIObject*   obj =   (*itr);
            obj->render();
        }
    }
};

void    TextDown(UIObject*)
{

}
void    TextClick(UIObject*)
{

}
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
    UIManager   _uiManager;
    UIButton    _button1;
    FreeTypeFont*_font;
public:
    SamplerTexture() 
    {
        _bDownFlag  =   false;
        _rolePos    =   float3(0,0,0);
        _moveTo     =   _rolePos;
        _angle      =   0;

        _button1._pos   =   float2(100,100);
        _button1._size  =   float2(100,40);

        _uiManager.push_back(&_button1);

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

        glDisable(GL_BLEND);

        _uiManager.render();

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

                _uiManager.onMessage(WM_LBUTTONDOWN,_mouseDown);
            }
            break;
        case WM_LBUTTONUP:
            {
                _bDownFlag  =   false;
                _uiManager.onMessage(WM_LBUTTONUP,int2(LOWORD(lParam),HIWORD(lParam)));
            }
            break;

        case WM_RBUTTONDOWN:
            {
                _mouseDown  =   int2(LOWORD(lParam),HIWORD(lParam));
                
            }
            break;

        case WM_RBUTTONUP:
            {
                _mouseDown  =   int2(LOWORD(lParam),HIWORD(lParam));
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
                int2    mouseCur    =  int2(LOWORD(lParam),HIWORD(lParam));
                if (_bDownFlag)
                {
                    float   xOffset     =  mouseCur.x - _mouseDown.x;
                    _mouseDown  =   mouseCur;
                    _camera.rotateY(xOffset * 0.5f);

                }
                _uiManager.onMessage(WM_MOUSEMOVE,mouseCur);
                
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