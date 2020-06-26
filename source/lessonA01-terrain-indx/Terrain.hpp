#pragma once


struct  TerrainVert
{
    float   x,y,z;
};


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

    void    setup(float w,float h)
    {
        float   step    =   1;
        float   startX  =   -w * 0.5f;
        float   startZ  =   -h * 0.5f;

        float   halfW   =   w * 0.5f;
        float   halfH   =   h * 0.5f;

        _drawSize       =   (w/step) * 2;
        
        /**
        *   产生顶点数据
        */
        for (float z = startZ ; z <= halfW ; z += step)
        {
            for (float x = startX ; x <= halfH ; x += step)
            {
                TerrainVert vert0   =   {x,         0,  z};
                _vertex.push_back(vert0);
            }
        }

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
        
    }

    void    render()
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3,GL_FLOAT,         sizeof(TerrainVert),     &_vertex[0].x);

        glDrawElements(GL_TRIANGLES,_index.size(),GL_UNSIGNED_SHORT,&_index.front());
    }
};
