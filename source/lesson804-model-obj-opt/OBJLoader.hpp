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


    struct  FaceIndex
    {
        unsigned short p1,p2,p3;
    };
    struct  Material
    {
        float3  ambient;
        float3  diffuse;
        float3  specal;
        char    szTex[128];
        GLuint  texId;
    };

    struct  OBJVert
    {
        float   x,y,z;
        float   nx,ny,nz;
        float   u,v;
    };

public:

    Material                    _material;

    std::vector<FaceIndex>      _indexs;

    std::vector<OBJVert>        _vertex;
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


        std::vector<CELL::float3>   _vertexs;
        std::vector<CELL::float3>   _normals;
        std::vector<CELL::float2>   _uvs;

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
            FaceIndex   index;
            {
                OBJVert vert;
                Face&   f   =   arFace[i];
                float3  v1   =   _vertexs[f.v1];
                float3  n1   =   _normals[f.n1];
                float2  t1   =   _uvs[f.t1];

                vert.x      = v1.x;
                vert.y      = v1.y;
                vert.z      = v1.z;
                vert.nx     = n1.x;
                vert.ny     = n1.y;
                vert.nz     = n1.z;

                vert.u     = t1.x;
                vert.v     = t1.y;
                
                //! 首先进行查找,如果在_vertex 数据里面,说明已经存在,否则加入
                int     pos =   find(vert);
                if (pos == -1 )
                {
                    pos =   _vertex.size();
                    _vertex.push_back(vert);
                }
                index.p1    =   pos;
            }
            {
                OBJVert vert;
                Face&   f   =   arFace[i];
                float3  v1   =   _vertexs[f.v2];
                float3  n1   =   _normals[f.n2];
                float2  t1   =   _uvs[f.t2];

                vert.x      = v1.x;
                vert.y      = v1.y;
                vert.z      = v1.z;
                vert.nx     = n1.x;
                vert.ny     = n1.y;
                vert.nz     = n1.z;

                vert.u     = t1.x;
                vert.v     = t1.y;

                //! 首先进行查找,如果在_vertex 数据里面,说明已经存在,否则加入
                int     pos =   find(vert);
                if (pos == -1 )
                {
                    pos =   _vertex.size();
                    _vertex.push_back(vert);
                }
                index.p2    =   pos;
            }

            {
                OBJVert vert;
                Face&   f   =   arFace[i];
                float3  v1   =   _vertexs[f.v3];
                float3  n1   =   _normals[f.n3];
                float2  t1   =   _uvs[f.t3];

                vert.x      = v1.x;
                vert.y      = v1.y;
                vert.z      = v1.z;
                vert.nx     = n1.x;
                vert.ny     = n1.y;
                vert.nz     = n1.z;

                vert.u     = t1.x;
                vert.v     = t1.y;

                //! 首先进行查找,如果在_vertex 数据里面,说明已经存在,否则加入
                int     pos =   find(vert);
                if (pos == -1 )
                {
                    pos =   _vertex.size();
                    _vertex.push_back(vert);
                }
                index.p3    =   pos;
            }

            _indexs.push_back(index);
        }
     
        fclose(pFile);
        return  true;
    }

    int     find(const OBJVert& vert)
    {
        for (int i = 0 ;i < _vertex.size(); ++ i)
        {
            OBJVert&    cur =   _vertex[i];
            if (  vert.x == cur.x
                &&vert.y == cur.y
                &&vert.z == cur.z
                &&vert.nx == cur.nx
                &&vert.ny == cur.ny
                &&vert.nz == cur.nz
                &&vert.u == cur.u
                &&vert.v == cur.v)
            {
                return  i;
            }
        }
        return  -1;
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
        glEnable(GL_LIGHT0);							
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);

        GLfloat diffuse_light1[] = { 0.25f, 0.25f, 0.25f, 1.0f };
        GLfloat position_light1[] = { 0.3f, 100, 0.2f, 0.0f };

        glLightfv( GL_LIGHT0, GL_POSITION, position_light1 );

        float   ambient[]   =   {0.588235,0.831373,0.945098,1};
        float   diffuse[]   =   {0.588235,0.831373,0.945098,1};
        float   specular[]  =   {0.9,0.9,0.9,1};


        GLfloat position_light0[] = { 0.0f, 100.0f, 0.0f, 0.0f };
        GLfloat diffuse_light0[]  = {  1.0f, 1.0f, 1.0f, 1.0f };
        GLfloat specular_light0[] = {  1.0f, 1.0f, 1.0f, 1.0f };
        glLightfv( GL_LIGHT0, GL_POSITION, position_light0 );
        glLightfv( GL_LIGHT0, GL_DIFFUSE,  diffuse_light0 );
        glLightfv( GL_LIGHT0, GL_SPECULAR, specular_light0 );

        // Be careful when setting up ambient lighting. You can very easily wash 
        // out the material's specular highlights.

        GLfloat ambient_lightModel[] = { 0.4f, 0.4f, 0.4f, 1.0f };

        glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );

        GLfloat g_greenPlasticMtrl_diffuse[4];
        GLfloat g_greenPlasticMtrl_ambient[4];
        GLfloat g_greenPlasticMtrl_specular[4];
        GLfloat g_greenPlasticMtrl_shininess = 64;


        g_greenPlasticMtrl_diffuse[0] = 0.0f;
        g_greenPlasticMtrl_diffuse[1] = 1.0f;
        g_greenPlasticMtrl_diffuse[2] = 0.0f;
        g_greenPlasticMtrl_diffuse[3] = 1.0f;

        // Lets favor the ambient's green over the other colors. Why? I don't know.
        // It just looks better to me. Using materials is some what more artistic
        // than scientific, so just play around till you get what you want.
        g_greenPlasticMtrl_ambient[0] = 0.5f;
        g_greenPlasticMtrl_ambient[1] = 1.0f;
        g_greenPlasticMtrl_ambient[2] = 0.5f;
        g_greenPlasticMtrl_ambient[3] = 1.0f;

        // Plastic can be shiny, but we don't want it too shiny are it will look 
        // more like glass or metal. We'll have the material reflect back more 
        // green than red and blue so the highlights won't be pure white.
        g_greenPlasticMtrl_specular[0] = 0.5f;
        g_greenPlasticMtrl_specular[1] = 1.0f;
        g_greenPlasticMtrl_specular[2] = 0.5f;
        g_greenPlasticMtrl_specular[3] = 1.0f;

        // It seems backwards, but increasing the shininess value reduces the 
        // highlight's size
        g_greenPlasticMtrl_shininess = 40.0f;

        glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, g_greenPlasticMtrl_diffuse );
        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, g_greenPlasticMtrl_ambient );
        glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, g_greenPlasticMtrl_specular );
        glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, g_greenPlasticMtrl_shininess );



        glBindTexture(GL_TEXTURE_2D,_material.texId);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3,GL_FLOAT,         sizeof(OBJVert),     &_vertex[0].x);
        glNormalPointer(GL_FLOAT,           sizeof(OBJVert),     &_vertex[0].nx);

        glDrawElements(GL_TRIANGLES,_indexs.size() * 3,GL_UNSIGNED_SHORT,&_indexs.front());

    }
};