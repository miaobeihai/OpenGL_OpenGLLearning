#pragma once


struct  TerrainVert
{
    
    float   x,y,z;
    float   nx,ny,nz;
    float   r,g,b,a;
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

public:

    Terrain(void)
    {
    }

    ~Terrain(void)
    {
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
   
    float   readHeight(int x,int y,const BYTE* data)
    {
        x = x % MAP_SIZE;
        y = y % MAP_SIZE;

        return (float)data[x + (y * MAP_SIZE)];
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
                vert0.x -=  MAP_SIZE* 0.5f;
                vert0.z -=  MAP_SIZE* 0.5f;
                vert0.r =   1;
                vert0.g =  1 ;//readColor(x,z,data);
                vert0.b =   1;
                vert0.a =   1;
                vert0.nx    =   0;
                vert0.ny    =   0;
                vert0.nz    =   0;
                _vertex.push_back(vert0);
            }
        }

        delete  []data;

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

            v0.nx   +=   nor.x;
            v0.ny   +=   nor.y;
            v0.nz   +=   nor.z;

            v1.nx   +=   nor.x;
            v1.ny   +=   nor.y;
            v1.nz   +=   nor.z;

            v2.nx   =   nor.x;
            v2.ny   =   nor.y;
            v2.nz   =   nor.z;
        }
    }

    void    render()
    {
        glBindTexture(GL_TEXTURE_2D,0);
        glEnableClientState(GL_VERTEX_ARRAY);
        //glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3,GL_FLOAT,         sizeof(TerrainVert),     &_vertex[0].x);
        glNormalPointer(GL_FLOAT,           sizeof(TerrainVert),     &_vertex[0].nx);
        //glColorPointer(4,GL_FLOAT,          sizeof(TerrainVert),     &_vertex[0].r);

        glDrawElements(GL_TRIANGLES,_index.size(),GL_UNSIGNED_SHORT,&_index.front());

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);


    }
};
