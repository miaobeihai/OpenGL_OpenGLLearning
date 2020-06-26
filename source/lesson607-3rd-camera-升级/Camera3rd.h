#pragma once

#include "CELLMath.hpp"
using namespace CELL;


class Camera3rd
{

public:
    float3  _eye;
    float3  _lookat;
    //! 定义了摄像机的y轴
    float3  _up;
    float3  _right;
    float   _speed;
    float   _radius;

    matrix4 _matView;
    matrix4 _matProj;
public:

    Camera3rd(void)
    {
        _eye    =   float3(0,10,10);
        _lookat =   float3(0,0,0);
        _radius =   10;
        updateLookat(_lookat);

        _up     =   float3(0,1,0);
        _right  =   float3(1,0,0);
        _speed  =   0.2f;
        
    }

    float   getRadius() const
    {
        return  _radius;
    }

    void    setRadius(float radius)
    {
        _radius =   radius;
    }


    matrix4 getMVP()
    {
        return  _matProj * _matView;
    }

    void    perspective(float fovy, float aspect, float zNear, float zFar)
    {
        _matProj    =   CELL::perspective(fovy,aspect,zNear,zFar);
    }
    void    updateLookat(float3 pos)
    {
        float3  dir =   normalize(_lookat - _eye);
        _lookat =   pos;
        _eye    =   _lookat - _radius * dir;
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

    void    update()
    {
        _matView    =   lookAt(_eye,_lookat,_up);
    }

    void    rotateY(float angle)
    {
        float3  dir =   normalize(_lookat - _eye);

        float3  dir1=   CELL::rotateY(dir,angle);

         _eye   =   _lookat  - dir1 * _radius;

        _right  =   normalize(cross(dir1 , _up));
    }
    virtual ~Camera3rd(void)
    {
    }
};
