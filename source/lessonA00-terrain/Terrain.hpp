#pragma once


struct  TerrainVert
{
    float   x,y,z;
};


class Terrain
{
public:
    typedef std::vector<TerrainVert>    ArrayVertex;
public:
    ArrayVertex _vertex;
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
        
        for (float z = startZ ; z < halfW ; z += step)
        {
            for (float x = startX ; x < halfH ; x += step)
            {
                TerrainVert vert0   =   {x,         0,  z       };     
                TerrainVert vert1   =   {x,         0,  z + step};

                _vertex.push_back(vert0);
                _vertex.push_back(vert1);
            }
        }
    }

    void    render()
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3,GL_FLOAT,         sizeof(TerrainVert),     &_vertex[0].x);
      
        for (size_t i = 0 ;i < _vertex.size() ; i += _drawSize)
        {
            glDrawArrays(GL_TRIANGLE_STRIP,i,_drawSize);
        }
    }
};
