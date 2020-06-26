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
        

        std::vector<Face>   arFace;
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

                face.v1 -=  1;
                face.v2 -=  1;
                face.v3 -=  1;
                face.n1 -=  1;
                face.n2 -=  1;
                face.n3 -=  1;
                face.t1 -=  1;
                face.t2 -=  1;
                face.t3 -=  1;
                arFace.push_back(face);
            }
        }

        std::vector<CELL::float3>   vertexs;
        std::vector<CELL::float2>   uvs;
        std::vector<CELL::float3>   normals;
        for (size_t i = 0 ;i < arFace.size() ; ++ i)
        {
            Face&   f   =   arFace[i];
            float3  v1   =   _vertexs[f.v1];
            float3  v2   =   _vertexs[f.v2];
            float3  v3   =   _vertexs[f.v3];

            vertexs.push_back(v1);
            vertexs.push_back(v2);
            vertexs.push_back(v3);

            float3  n1   =   _normals[f.n1];
            float3  n2   =   _normals[f.n2];
            float3  n3   =   _normals[f.n3];

            normals.push_back(n1);
            normals.push_back(n2);
            normals.push_back(n3);

            float2  t1   =   _uvs[f.t1];
            float2  t2   =   _uvs[f.t2];
            float2  t3   =   _uvs[f.t3];

            uvs.push_back(t1);
            uvs.push_back(t2);
            uvs.push_back(t3);
        }

        _normals    =   normals;
        _vertexs    =   vertexs;
        _uvs        =   uvs;
        fclose(pFile);
        return  true;
    }

};


class   OBJRender :public OBJLoader
{
public:

    void    render(CELL::matrix4& mvp)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3,GL_FLOAT,         0,     &_vertexs.front());
        glNormalPointer(GL_FLOAT,           0,     &_normals.front());
        glTexCoordPointer(2,GL_FLOAT,       0,     &_uvs[0]);

        glDrawArrays(GL_TRIANGLES,0,_vertexs.size());

    }
};