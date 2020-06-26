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

    struct  Material
    {
        float3  ambient;
        float3  diffuse;
        float3  specal;
        char    szTex[128];
        GLuint  texId;
    };

public:

    std::vector<CELL::float3>   _vertexs;
    std::vector<CELL::float3>   _normals;
    std::vector<CELL::float2>   _uvs;
    Material                    _material;
public:

    OBJLoader()
    {
        memset(&_material,0,sizeof(_material));

    }

    /**
    *   加载文件
    */

    virtual bool    load(const char* fileName)
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

            if (strncmp(szLine,"mtllib",6) == 0)
            {
                char    matFile[128];
                sscanf(szLine,"mtllib %s",matFile);
                parseMaterial(matFile);
            }
            //! 顶点数据
            else if (szLine[0] == 'v' && szLine[1] == ' ')
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


    virtual bool    parseMaterial(const char* fileName)
    {
        FILE*   pFile   =   fopen(fileName,"rt");
        if (pFile == 0)
        {
            return  false;
        }

        while (feof(pFile) == 0)
        {
            char    szLine[1024];
            fgets(szLine,sizeof(szLine),pFile);

            if (strncmp(szLine,"Ka",2) == 0)
            {
                sscanf(szLine,"Ka  %f %f %f"
                    ,&_material.ambient.x
                    ,&_material.ambient.y
                    ,&_material.ambient.z);
            }
            else if (strncmp(szLine,"Kd",2) == 0)
            {
                sscanf(szLine,"Kd  %f %f %f"
                    ,&_material.diffuse.x
                    ,&_material.diffuse.y
                    ,&_material.diffuse.z);
            }
            else if (strncmp(szLine,"Ks",2) == 0)
            {
                sscanf(szLine,"Ks  %f %f %f"
                    ,&_material.specal.x
                    ,&_material.specal.y
                    ,&_material.specal.z);
            }
            else if (strncmp(szLine,"map_Kd",6) == 0)
            {
                sscanf(szLine,"map_Kd %s",_material.szTex);
            }
        }
        fclose(pFile);

        return  true;

    }

};


class   OBJRender :public OBJLoader
{
public:

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

    unsigned    createTexture(int w,int h,const void* data)
    {
        unsigned    texId;
        glGenTextures(1,&texId);
        glBindTexture(GL_TEXTURE_2D,texId);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,data);

        return  texId;
    }

    bool    load(const char* fileName)
    {
        bool    res =   OBJLoader::load(fileName);
        if (res && strlen(_material.szTex) > 0)
        {
            _material.texId =   createTextureFromImage(_material.szTex);
        }

        return  res;
    }
    void    render(CELL::matrix4& mvp)
    {
        if (_material.texId != 0)
        {
            glBindTexture(GL_TEXTURE_2D,_material.texId);
        }


        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3,GL_FLOAT,         0,     &_vertexs.front());
        glNormalPointer(GL_FLOAT,           0,     &_normals.front());
        glTexCoordPointer(2,GL_FLOAT,       0,     &_uvs[0]);

        glDrawArrays(GL_TRIANGLES,0,_vertexs.size());

    }
};