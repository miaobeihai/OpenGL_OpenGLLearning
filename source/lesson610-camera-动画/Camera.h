#pragma once

#include "CELLMath.hpp"
using namespace CELL;
class Camera
{

public:
    float3  _eye;
    float3  _lookat;
    //! 定义了摄像机的y轴
    float3  _up;
    float3  _right;
    float   _speed;

    matrix4 _matView;
    matrix4 _matProj;
    float2  _viewSize;

public:

    Camera(void)
    {
        _eye    =   float3(0,0,10);
        _lookat =   float3(0,0,0);
        _up     =   float3(0,1,0);
        _right  =   float3(1,0,0);
        _speed  =   0.2f;
    }

    void    moveLeft()
    {
        _eye    -=  normalize(_right) * _speed;
        _lookat -=  normalize(_right) * _speed;
    }
    void    moveRight()
    {
        _eye    +=  normalize(_right) * _speed;
        _lookat +=  normalize(_right) * _speed;
    }

    void    moveFront()
    {
        float3  dir =   normalize(_lookat - _eye);
        _eye    +=  dir * _speed;
        _lookat +=  dir * _speed;

    }
    void    moveBack()
    {
        float3  dir =   normalize(_lookat - _eye);
        _eye    -=  dir * _speed;
        _lookat +=  dir * _speed;
    }
    void    setView(int x,int y)
    {
        _viewSize.x =   x;
        _viewSize.y =   y;
    }

    matrix4 getMVP()
    {
        return  _matProj * _matView;
    }

    void    update()
    {
        _matView    =   lookAt(_eye,_lookat,_up);
    }

    void    perspective(float fovy, float aspect, float zNear, float zFar)
    {
        _matProj    =   CELL::perspective(fovy,aspect,zNear,zFar);
    }

    void    rotateY(float angle)
    {
        float3  dir =   normalize(_lookat - _eye);
        float   len =   length(_lookat - _eye);

        float3  dir1=   CELL::rotateY(dir,angle);

        _lookat =   _eye + dir1 * len;

        _right  =   normalize(cross(dir1 , _up));
    }
    virtual ~Camera(void)
    {
    }
};
