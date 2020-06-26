
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "CELLMath.hpp"

#include "OpenGLWindow.h"
#include <vector>
using namespace CELL;

class   SamplerSplineEditor :public OpenGLWindow
{
public:

    tspline<float> _spline;
    bool            _isButtonDown;
    size_t          _selectIndex;

    POINT           _mouseOld;

    SamplerSplineEditor() 
    {
        _isButtonDown   =   false;
        _selectIndex    =   -1;

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

        for (int i = 0 ;i < _spline.getNumPoints() ; ++ i)
        {
            _keys.push_back(_spline.getPoint(i));
        }
    }

    std::vector<float3> _array;
    std::vector<float3> _keys;

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

        glPointSize(8);
        glVertexPointer(3,GL_FLOAT,sizeof(float3),&_keys[0]);
        glDrawArrays(GL_POINTS,0, _keys.size());
        


    }

    /**
    *   消息处理过程
    */
    virtual LRESULT events(HWND hWnd,UINT msgId, WPARAM wParam, LPARAM lParam)
    {
        switch(msgId)
        {
        case WM_CLOSE:
        case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
            break;
        case  WM_LBUTTONDOWN:
            {
                _isButtonDown   =   true;
                _selectIndex    =   -1;

                POINT   pt  =  {LOWORD(lParam),HIWORD(lParam)};

                _mouseOld   =   pt;

                for (size_t i = 0 ;i< _keys.size() ; ++ i)
                {
                    RECT    rt;
                    rt.left =   _keys[i].x - 4;
                    rt.right =   _keys[i].x + 4;
                    rt.top =   _keys[i].y - 4;
                    rt.bottom =   _keys[i].y + 4;

                    if (PtInRect(&rt,pt))
                    {
                        _selectIndex    =   i;
                        break;
                    }
                }
            }
            break;
        case WM_LBUTTONUP:
            {
                _isButtonDown   =   false;
                _selectIndex    =   -1;
            }
            break;
        case WM_MOUSEMOVE:
            {
                if (_isButtonDown && _selectIndex != -1)
                {
                    POINT   pt  =   {LOWORD(lParam),HIWORD(lParam)};

                    POINT   off =   {   pt.x - _mouseOld.x,pt.y - _mouseOld.y};

                    _mouseOld   =   pt;

                    _keys[_selectIndex].x   +=  off.x;
                    _keys[_selectIndex].y   +=  off.y;

                    _spline.getPoint(_selectIndex).x    +=  off.x;
                    _spline.getPoint(_selectIndex).y    +=  off.y;

                    _spline.recalcTangents();

                    _array.clear();

                    for (float t = 0 ; t < 1.0f; t += 0.01f)
                    {
                        float3  pos =   _spline.interpolate(t);
                        _array.push_back(pos);
                    }

                }
            }
            break;
        default:
            return DefWindowProc(hWnd, msgId, wParam, lParam);
        }
        return  0;
    }

};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerSplineEditor    instance;
    instance.main(800,600);


    return  0;
}