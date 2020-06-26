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
    matrix4 getMVP()
    {
        return  _matProj * _matView;
    }
    void    perspective(float fovy, float aspect, float zNear, float zFar)
    {
        _matProj    =   CELL::perspective(fovy,aspect,zNear,zFar);
    }

    /**
    *   窗口坐标转化为世界坐标
    */
    bool    unProject( const float4& screen, float4& world )
    {
        float4 v;
        v.x =   screen.x;
        v.y =   screen.y;
        v.z =   screen.z;
        v.w =   1.0;

        // map from viewport to 0 - 1
        v.x =   (v.x) /_viewSize.x;
        v.y =   (_viewSize.y - v.y) /_viewSize.y;
        //v.y = (v.y - _viewPort.Y) / _viewPort.Height;

        // map to range -1 to 1
        v.x =   v.x * 2.0f - 1.0f;
        v.y =   v.y * 2.0f - 1.0f;
        v.z =   v.z * 2.0f - 1.0f;

        CELL::matrix4  inverse = (_matProj * _matView ).inverse();

        v   =   v * inverse;
        if (v.w == 0.0f)
        {
            return false;
        }
        world   =   v / v.w;
        return true;
    }


    Ray     createRayFromScreen(int x,int y)
    {
        float4  minWorld;
        float4  maxWorld;

        float4  screen(float(x),float(y),0,1);
        float4  screen1(float(x),float(y),1,1);

        unProject(screen,minWorld);
        unProject(screen1,maxWorld);
        Ray     ray;
        ray.setOrigin(float3(minWorld.x,minWorld.y,minWorld.z));

        float3  dir(maxWorld.x - minWorld.x,maxWorld.y - minWorld.y, maxWorld.z - minWorld.z);
        ray.setDirection(normalize(dir));
        return  ray;
    }
    void    setView(int x,int y)
    {
        _viewSize.x =   x;
        _viewSize.y =   y;
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

    /**
    *   更新摄像机的 Y
    */
    void    updateH(float height)
    {
        float   offset  =   height - _eye.y;
        _eye.y      =   height;  
        _lookat.y   +=  offset;
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
