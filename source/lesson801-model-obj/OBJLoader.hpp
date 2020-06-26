#pragma once

#include    "CELLMath.hpp"
#include    <vector>
class   OBJLoader
{
public:

    struct  Face
    {
        unsigned int  v1,v2,v3;
        unsigned int  n1,n2,n3;
        unsigned int  t1,t2,t3;
    };
public:

    std::vector<CELL::float3>   _vertexs;
    std::vector<CELL::float3>   _normals;
    std::vector<CELL::float2>   _uvs;

    std::vector<Face>           _face;   
public:

    /**
    *   加载文件
    */

    bool    load(const char* fileName)
    {
        FILE*   pFile   =   fopen(fileName,"rt");
        if (pFile == 0)
        {
            return  false;
        }

        _vertexs.clear();
        _uvs.clear();
        _normals.clear();
        _face.clear();
        while (feof(pFile) == 0)
        {
            char    szLine[1024];
            fgets(szLine,sizeof(szLine),pFile);
            //! 顶点数据
            if (szLine[0] == 'v' && szLine[1] == ' ')
            {
                CELL::float3    pos;
                sscanf(szLine,"v  %f %f %f",&pos.x,&pos.y,&pos.z);
                _vertexs.push_back(pos);
            }
            else if(szLine[0] == 'v' && szLine[1] == 't')
            {
                CELL::float2    pos;
                sscanf(szLine,"vt  %f %f",&pos.x,&pos.y);
                _uvs.push_back(pos);
            }
            else if(szLine[0] == 'v' && szLine[1] == 'n')
            {
                CELL::float3    pos;
                sscanf(szLine,"vn  %f %f %f",&pos.x,&pos.y,&pos.z);
                _normals.push_back(pos);
            }
            else if(szLine[0] == 'f' && szLine[1] == ' ')
            {
                Face    face;
                sscanf(szLine,
                    "f %d/%d/%d %d/%d/%d %d/%d/%d"
                    ,&face.v1
                    ,&face.t1
                    ,&face.n1
                    ,&face.v2
                    ,&face.t2
                    ,&face.n2 
                    ,&face.v3
                    ,&face.t3
                    ,&face.n3);
                _face.push_back(face);
            }
        }
        fclose(pFile);
        return  true;
    }

};