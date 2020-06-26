#pragma once

struct  TerrainVert
{
    float   x,y,z;
    float   nx,ny,nz;
    float   u,v;
    float   u1,v1;
    float   r,g,b,a;
    float   fog;
};


#define MAP_SIZE    1024
class Terrain
{
public:
    typedef std::vector<TerrainVert>    ArrayVertex;
    typedef std::vector<unsigned short> ArrayIndex;
public:
    ArrayVertex _vertex;
    ArrayIndex  _index;
    int         _drawSize;
    BYTE*       _data;

public:

    Terrain(void)
    {
        _data   =   0;
    }

    ~Terrain(void)
    {
        if (_data)
        {
            delete  _data;
        }
    }


    bool    loadHeight(const char* fileName,BYTE* pHeightMap,int w,int h)
    {
        FILE*   pFile   =   fopen( fileName, "rb" );
        if ( pFile == NULL )	
        {
            return  false;
        }
        fread( pHeightMap, 1, w * h, pFile );
        fclose(pFile);
        return  true;
        
    }

    bool    loadHeight(const char* fileName,BYTE* pHeightMap)
    {
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

        USHORT*   pixels =   (USHORT*)FreeImage_GetBits(dib);
        int     width   =   FreeImage_GetWidth(dib);
        int     height  =   FreeImage_GetHeight(dib);
        return  0;

    }
   
    float   readHeight(int x,int y,const BYTE* data)
    {
        x = x % MAP_SIZE;
        y = y % MAP_SIZE;

        return (float)data[x + (y * MAP_SIZE)];
    }

    float   readFog(int x,int y,const BYTE* data)
    {
        float h = readHeight(x,y,data);
        if (h > 120)
        {
            return  0;
        }
        return   (120 - h);
    }

    float    readColor(int x,int y,const BYTE* data)
    {
       return -0.15f + (readHeight( x, y,data ) / 256.0f);
    }

    
    float3  computeNomal(TerrainVert& v1, TerrainVert& v2, TerrainVert& v3)
    {
          float3    f1(v2.x - v1.x,    v2.y - v1.y,    v2.z - v1.z);
          float3    f2(v3.x - v1.x,    v3.y - v1.y,    v3.z - v1.z);
          return    normalize(cross(f1, f2));
    }

    /**
    *   根据给定的x,z世界坐标，计算出来所在位置的高度
    */
    float   calcHeight(float x,float z,BYTE* data)
    {
        float   startX  =   -MAP_SIZE * 0.5f;
        float   startY  =   -MAP_SIZE * 0.5f;
        float   offsetX =   x - startX;
        float   offsetZ =   z - startY;

        if (offsetX < 0)
        {
            offsetX =   0;
        }
        if (offsetZ < 0)
        {
            offsetZ =   0;
        }
        if (offsetX >= MAP_SIZE)
        {
            offsetX =   MAP_SIZE - 1;
        }
        if (offsetZ >= MAP_SIZE)
        {
            offsetZ =   MAP_SIZE - 1;
        }

        int     posX    =   (int)offsetX;
        int     posZ    =   (int)offsetZ;

        return  readHeight(posX,posZ,data);
    }
    void    setup(float w,float h)
    {
        int     step    =   8;
       
        BYTE*   data    =   new BYTE[MAP_SIZE * MAP_SIZE];
        loadHeight("Terrain.raw",data,MAP_SIZE,MAP_SIZE);
        
        /**
        *   产生顶点数据
        */
        for (int z = 0 ; z <= MAP_SIZE ; z += step)
        {
            for (int x = 0 ; x <= MAP_SIZE ; x += step)
            {
                TerrainVert vert0   =   {x, readHeight(x,z,data),  z, 0,0,0,1};
                vert0.x     -=  MAP_SIZE* 0.5f;
                vert0.z     -=  MAP_SIZE* 0.5f;
                vert0.r     =   1;
                vert0.g     =   1;
                vert0.b     =   1;
                vert0.a     =   1;
                vert0.u     =   float(x)/MAP_SIZE;
                vert0.v     =   float(z)/MAP_SIZE;

                vert0.u1     =   vert0.u * 20;
                vert0.v1     =   vert0.v  * 20;

                vert0.fog   =   readFog(x,z,data);

                _vertex.push_back(vert0);
            }
        }

        _data   =   data;

        /**
        *   计算索引数据
        */
        size_t  col =   int(w/step + 0.5f) + 1;
        size_t  row =   int(h/step + 0.5f) + 1;

        for (size_t r = 0 ;r < row - 1 ; ++ r)
        {
            for (size_t i = 0 ; i < col - 1 ; ++ i)
            {
                _index.push_back((r + 0) * col + i);
                _index.push_back((r + 1) * col + i);
                _index.push_back((r + 0) * col + i + 1);

                _index.push_back((r + 1) * col + i);
                _index.push_back((r + 0) * col + i + 1);
                _index.push_back((r + 1) * col + i + 1);
            }
        }

        /**
        *   计算法线
        */
        for (size_t i = 0 ;i < _index.size() ; i+= 3)
        {
            TerrainVert&    v0  =   _vertex[_index[i + 0]];
            TerrainVert&    v1  =   _vertex[_index[i + 1]];
            TerrainVert&    v2  =   _vertex[_index[i + 2]];
            float3          nor =   computeNomal(v0,v1,v2);

            v0.nx   =   nor.x;
            v0.ny   =   nor.y;
            v0.nz   =   nor.z;

            v1.nx   =   nor.x;
            v1.ny   =   nor.y;
            v1.nz   =   nor.z;

            v2.nx   =   nor.x;
            v2.ny   =   nor.y;
            v2.nz   =   nor.z;
        }
    }

    void    render()
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_FOG_COORD_ARRAY);
        glVertexPointer(3,GL_FLOAT,         sizeof(TerrainVert),     &_vertex[0].x);
        glNormalPointer(GL_FLOAT,           sizeof(TerrainVert),     &_vertex[0].nx);
        glColorPointer(4,GL_FLOAT,          sizeof(TerrainVert),     &_vertex[0].r);
        //glTexCoordPointer(2,GL_FLOAT,       sizeof(TerrainVert),     &_vertex[0].u);

        glClientActiveTextureARB(GL_TEXTURE0_ARB);
        glTexCoordPointer(2,GL_FLOAT,       sizeof(TerrainVert),     &_vertex[0].u);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glClientActiveTextureARB(GL_TEXTURE1_ARB);
        glTexCoordPointer(2,GL_FLOAT,       sizeof(TerrainVert),     &_vertex[0].u1);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glFogCoordPointer(GL_FLOAT,         sizeof(TerrainVert),     &_vertex[0].fog);

        glDrawElements(GL_TRIANGLES,_index.size(),GL_UNSIGNED_SHORT,&_index.front());

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_FOG_COORD_ARRAY);


    }
};
