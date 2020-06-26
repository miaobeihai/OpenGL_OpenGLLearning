#pragma once

#include "CELLMath.hpp"
#include "freetype/ftglyph.h"
#pragma comment(lib,"freetype.lib")

#include FT_GLYPH_H
#include FT_TRUETYPE_TABLES_H
#include FT_BITMAP_H
#include FT_WINFONTS_H


using namespace CELL;

struct  FontVertex
{
    float       x,y,z;
    float       u,v;
    Rgba4Byte   color;
};


class   Character
{
public:
    Character()
    {
        x0          =   0;
        y0          =   0;
        x1          =   0;
        y1          =   0;
        offsetX     =   0;
        offsetY     =   0;
    }
    /**
    *   存储当前字符在纹理上的坐标位置
    *   采用1024大小的纹理，可以存常用的汉字(16像素)
    */
    unsigned __int64   x0:10;
    unsigned __int64   y0:10;
    unsigned __int64   x1:10;
    unsigned __int64   y1:10;
    //! 字体的偏移x方向
    unsigned __int64   offsetX:8;
    //! 字体的偏移y方向
    unsigned __int64   offsetY:8;
    //! 纹理的所有，最大256个
    unsigned __int64   texIndex:8;
};




class   FreeTypeFont
{
public:
    FT_Library      _library;
    FT_Face         _face; 
    unsigned        _sysFontTexture;
    Character       _character[1<<16];

    int             _textureWidth;
    int             _textureHeight;
    //! 临时变量，保存当前字体写到文理上的位置坐标
    int             _yStart;
    int             _xStart;
    //! 字体大小
    int             _fontSize;  
    //! 上述字体的大小并不是字体的像素大小，所以需要用一个变量来计算字体的像素大小
    int             _fontPixelX;
    int             _fontPixelY;
    bool            _isSymbol;
    char            _vertexBuffer[1024 * 64];
public:
    
    FreeTypeFont()
    {
        memset(_character,0,sizeof(_character));

        _library        =   0;
        _face           =   0; 
        _yStart         =   0;
        _xStart         =   0;
        _sysFontTexture =   0;
        _fontSize       =   0;
        _fontPixelX     =   0;
        _fontPixelY     =   0;
        _textureHeight  =   512;
        _textureWidth   =   512;
        _isSymbol       =   false;

        FT_Init_FreeType( (FT_Library*)&_library );
    }

    ~FreeTypeFont()
    {
        destroy();
    }

    void    destroy()
    {
        glDeleteTextures(1,&_sysFontTexture);
        FT_Done_Face(FT_Face(_face));
        _xStart     =   0;
        _yStart     =   0;
        _face       =   0;
        memset(_character,0,sizeof(_character));
    }
   

    unsigned    getTextureHandle()
    {
        return  _sysFontTexture;
    }

    int     getWidth()
    {
        return  _textureWidth;
    }

    int     getHeight()
    {
        return  _textureHeight;
    }
    /**
    *    新增加
    */
    void    begin(int width,int height)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,width,height,0,1,-1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D,_sysFontTexture);
    }

    void    end()
    {
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D,0);
    }
    /**
    *   创建字体
    */
    bool    create(const char* faceName,int fontSize,int w ,int h)
    {
        _textureWidth   =   w;
        _textureHeight  =   h;
        /**
        *   保存字体的大小
        */
        _fontSize   =   fontSize;
        /**
        *   已经创建了字体则销毁
        *   支持多次调用
        */
        if (_face)
        {
            FT_Done_Face(FT_Face(_face));
            _xStart         =   0;
            _yStart         =   0;
            memset(_character,0,sizeof(_character));
        }
        
        
        FT_Face     ftFace  =   0;
        FT_Error    error   =   FT_New_Face( (FT_Library)_library, faceName, 0, &ftFace );
                    _face   =   ftFace;
       
        if ( error != 0 ) 
        {
            return  false;
        }
        
        /**
        *   默认选择第一个字体
        */

        if ( ftFace->charmaps != 0 && ftFace->num_charmaps > 0 )
        {
            FT_Select_Charmap( ftFace, ftFace->charmaps[0]->encoding );
        }

        FT_Select_Charmap( ftFace, FT_ENCODING_UNICODE );
        FT_F26Dot6 ftSize = (FT_F26Dot6)(fontSize * (1 << 6));

        FT_Set_Char_Size(ftFace, ftSize, 0, 72, 72);
        /**
        *   销毁字体
        */
        if (_sysFontTexture != 0)
        {
            glDeleteTextures(1,&_sysFontTexture);
        }

        glGenTextures(1,&_sysFontTexture);
        /**
        *   使用这个纹理id,或者叫绑定(关联)
        */
        glBindTexture( GL_TEXTURE_2D, _sysFontTexture );
        /**
        *   指定纹理的放大,缩小滤波，使用线性方式，即当图片放大的时候插值方式 
        */
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        glTexImage2D( 
            GL_TEXTURE_2D,      //! 指定是二维图片
            0,                  //! 指定为第一级别，纹理可以做mipmap,即lod,离近的就采用级别大的，远则使用较小的纹理
            GL_ALPHA,           //! 纹理的使用的存储格式
            _textureWidth,
            _textureHeight,
            0,                  //! 是否的边
            GL_ALPHA,           //! 数据的格式，bmp中，windows,操作系统中存储的数据是bgr格式
            GL_UNSIGNED_BYTE,   //! 数据是8bit数据
            0
            );
        return  true;
    }

    
    Character getCharacter( int ch )
    {
        if (_character[ch].x0 == 0 &&
            _character[ch].x0 == 0 &&
            _character[ch].x1 == 0 &&
            _character[ch].y1 == 0
            )
        {

            glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

            if (_xStart + tmax<int>(_fontPixelX,_fontSize) > _textureWidth)
            {
                /**
                *   写满一行,从新开始
                */
                _xStart =   0;
                /**
                *   y开始位置要增加
                */
                _yStart +=  tmax<int>(_fontPixelY,_fontSize);
            }
            FT_Load_Glyph((FT_Face) _face, FT_Get_Char_Index( (FT_Face)_face, ch ), FT_LOAD_DEFAULT );
            FT_Glyph glyph;
            FT_Glyph glyphSrc;
            FT_Get_Glyph( FT_Face(_face)->glyph, &glyphSrc );

            glyph   =   glyphSrc;

            /**
            *   根据字体的大小决定是否使用反锯齿绘制模式
            *   当字体比较小的是说建议使用ft_render_mode_mono
            *   当字体比较大的情况下12以上，建议使用ft_render_mode_normal模式
            */
            FT_Error    err =   -1;
            if (_fontSize <= 16)
            {
                err =   FT_Glyph_To_Bitmap( &glyph, ft_render_mode_mono, 0, 1 );
            }
            else
            {
                if ( ch >= 0 && ch <=256 && !_isSymbol)
                {

                    err =   FT_Glyph_To_Bitmap( &glyph, ft_render_mode_mono, 0, 1 );
                }
                else
                {
                    err =   FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
                }
            }
            if(err != 0)
            {
            }
            
            FT_BitmapGlyph  bitmap_glyph    =   (FT_BitmapGlyph)glyph;
            FT_Bitmap&      bitmap          =   bitmap_glyph->bitmap;
            FT_Bitmap       ftBitmap;

            FT_Bitmap*      pBitMap         =   &bitmap;
           
            FT_Bitmap_New(&ftBitmap);

            if(bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
            {
                if (FT_Bitmap_Convert((FT_Library)_library, &bitmap, &ftBitmap, 1) == 0)
                {
                    /**
                    *   Go through the bitmap and convert all of the nonzero values to 0xFF (white).
                    */
                    for (unsigned char* p = ftBitmap.buffer, * endP = p + ftBitmap.width * ftBitmap.rows; p != endP; ++p)
                        *p ^= -*p ^ *p;
                    pBitMap  =   &ftBitmap;
                }
            }

            /**
            *   如果没有数据，则不写，直接过去
            */
            if (pBitMap->width == 0 || pBitMap->rows == 0)
            {
                
                char    mem[1024 * 32];
                memset(mem,0,sizeof(mem));

                _character[ch].x0       =   _xStart;
                _character[ch].y0       =   _yStart;
                _character[ch].x1       =   _xStart + _fontSize/2;
                _character[ch].y1       =   _yStart + _fontSize - 1;
                _character[ch].offsetY  =   _fontSize - 1;
                _character[ch].offsetX  =   0;

                

                glBindTexture(GL_TEXTURE_2D,_sysFontTexture);

                glTexSubImage2D (
                    GL_TEXTURE_2D,
                    0,
                    _xStart,
                    _yStart,
                    _fontSize/2,
                    _fontSize,
                    GL_ALPHA,
                    GL_UNSIGNED_BYTE,
                    mem
                    );
                _xStart +=  _fontSize/2;


            }
            else
            {
                glBindTexture(GL_TEXTURE_2D,_sysFontTexture);

                _character[ch].x0       =   _xStart;
                _character[ch].y0       =   _yStart;
                _character[ch].x1       =   _xStart + pBitMap->width;
                _character[ch].y1       =   _yStart + pBitMap->rows;

                _character[ch].offsetY  =   bitmap_glyph->top;
                _character[ch].offsetX  =   bitmap_glyph->left;

                glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
                glTexSubImage2D (
                    GL_TEXTURE_2D,
                    0,
                    _xStart,
                    _yStart,
                    tmax<int>(1,pBitMap->width),
                    tmax<int>(1,pBitMap->rows),
                    GL_ALPHA,
                    GL_UNSIGNED_BYTE,
                    pBitMap->buffer
                    );
                _xStart     +=  (pBitMap->width + 1);
                _fontPixelY =   tmax<int>(_fontPixelY,pBitMap->rows);
                _fontPixelX =   tmax<int>(_fontPixelX,pBitMap->width);
            }

            if (glyph != glyphSrc)
            {
                FT_Done_Glyph(glyph);
            }
            else
            {
                FT_Done_Glyph(glyphSrc);
            }

            FT_Bitmap_Done((FT_Library)_library,&ftBitmap);

        }
        return  _character[ch];
    }

    void    selectCharmap( unsigned charMap )
    {
        FT_Select_Charmap( FT_Face(_face), (FT_Encoding)charMap);
    }

    float2  drawText(int x,int y,int z,Rgba color,wchar_t* text,size_t length,FontVertex** vertexs,size_t*vertexLength = 0)
    {
        float           texWidth    =   float(_textureWidth);
        float           texHeight   =   float(_textureHeight);
        float           xStart      =   float(x);
        float           yStart      =   float(y + _fontSize);
        float           zStart      =   float(z);
        unsigned        index       =   0;
        unsigned        size        =   length == 0 ? wcslen(text) :length;
        float2          vSize(0,0);
        FontVertex*     vertex      =   (FontVertex*)_vertexBuffer;

        if (size == 0)
        {
            return  vSize;
        }
        for (unsigned i = 0 ;i <  size; ++ i )
        {
            Character   ch          =   getCharacter(text[i]);

            float       h           =   float(ch.y1 - ch.y0);
            float       w           =   float(ch.x1 - ch.x0);
            float       offsetY     =   (float(h) - float(ch.offsetY));
            /**
            *   第一个点
            */
            vertex[index + 0].x       =   xStart;
            vertex[index + 0].y       =   yStart - h + offsetY;
            vertex[index + 0].z       =   zStart;
            vertex[index + 0].u       =   ch.x0/texWidth;
            vertex[index + 0].v       =   ch.y0/texHeight;
            vertex[index + 0].color   =   color;
            /**
            *   第二个点
            */
            vertex[index + 1].x       =   xStart + w;
            vertex[index + 1].y       =   yStart - h + offsetY;
            vertex[index + 1].z       =   zStart;
            vertex[index + 1].u       =   ch.x1/texWidth;
            vertex[index + 1].v       =   ch.y0/texHeight;
            vertex[index + 1].color   =   color;
            /**
            *   第三个点
            */
            vertex[index + 2].x       =   xStart + w;
            vertex[index + 2].y       =   yStart  + offsetY;
            vertex[index + 2].z       =   zStart;
            vertex[index + 2].u       =   ch.x1/texWidth;
            vertex[index + 2].v       =   ch.y1/texHeight;
            vertex[index + 2].color   =   color;
            /**
            *   第一个点
            */
            vertex[index + 3].x       =   xStart;
            vertex[index + 3].y       =   yStart - h + offsetY;
            vertex[index + 3].z       =   zStart;
            vertex[index + 3].u       =   ch.x0/texWidth;
            vertex[index + 3].v       =   ch.y0/texHeight;
            vertex[index + 3].color   =   color;
            /**
            *   第三个点
            */
            vertex[index + 4].x       =   xStart + w;
            vertex[index + 4].y       =   yStart  + offsetY;
            vertex[index + 4].z       =   zStart;
            vertex[index + 4].u       =   ch.x1/texWidth;
            vertex[index + 4].v       =   ch.y1/texHeight;
            vertex[index + 4].color   =   color;
            /**
            *   第四个点
            */
            vertex[index + 5].x       =   xStart;
            vertex[index + 5].y       =   yStart  + offsetY;
            vertex[index + 5].z       =   zStart;
            vertex[index + 5].u       =   ch.x0/texWidth;
            vertex[index + 5].v       =   ch.y1/texHeight;
            vertex[index + 5].color   =   color;

            index   +=  6;
            xStart  +=  w + (ch.offsetX + 1);

            vSize.x +=  w + (ch.offsetX + 1);
            vSize.y =   tmax<float>(h + offsetY,vSize.y);

           

        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        {
            glVertexPointer(3,GL_FLOAT,         sizeof(FontVertex),     vertex);
            glTexCoordPointer(2,GL_FLOAT,       sizeof(FontVertex),     &vertex[0].u);
            glColorPointer(4,GL_UNSIGNED_BYTE,  sizeof(FontVertex),     &vertex[0].color);
            glDrawArrays(GL_TRIANGLES,0,index);
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        /**
        *   向外输出顶点数据.
        */
        if (vertexs)
        {
            *vertexs    =   vertex;
        }
        if (vertexLength)
        {
            *vertexLength   =   index;
        }
        return  vSize;
    }
};