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
    *   �洢��ǰ�ַ��������ϵ�����λ��
    *   ����1024��С���������Դ泣�õĺ���(16����)
    */
    unsigned __int64   x0:10;
    unsigned __int64   y0:10;
    unsigned __int64   x1:10;
    unsigned __int64   y1:10;
    //! �����ƫ��x����
    unsigned __int64   offsetX:8;
    //! �����ƫ��y����
    unsigned __int64   offsetY:8;
    //! ��������У����256��
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
    //! ��ʱ���������浱ǰ����д�������ϵ�λ������
    int             _yStart;
    int             _xStart;
    //! �����С
    int             _fontSize;  
    //! ��������Ĵ�С��������������ش�С��������Ҫ��һ��������������������ش�С
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
    *    ������
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
    *   ��������
    */
    bool    create(const char* faceName,int fontSize,int w ,int h)
    {
        _textureWidth   =   w;
        _textureHeight  =   h;
        /**
        *   ��������Ĵ�С
        */
        _fontSize   =   fontSize;
        /**
        *   �Ѿ�����������������
        *   ֧�ֶ�ε���
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
        *   Ĭ��ѡ���һ������
        */

        if ( ftFace->charmaps != 0 && ftFace->num_charmaps > 0 )
        {
            FT_Select_Charmap( ftFace, ftFace->charmaps[0]->encoding );
        }

        FT_Select_Charmap( ftFace, FT_ENCODING_UNICODE );
        FT_F26Dot6 ftSize = (FT_F26Dot6)(fontSize * (1 << 6));

        FT_Set_Char_Size(ftFace, ftSize, 0, 72, 72);
        /**
        *   ��������
        */
        if (_sysFontTexture != 0)
        {
            glDeleteTextures(1,&_sysFontTexture);
        }

        glGenTextures(1,&_sysFontTexture);
        /**
        *   ʹ���������id,���߽а�(����)
        */
        glBindTexture( GL_TEXTURE_2D, _sysFontTexture );
        /**
        *   ָ������ķŴ�,��С�˲���ʹ�����Է�ʽ������ͼƬ�Ŵ��ʱ���ֵ��ʽ 
        */
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        glTexImage2D( 
            GL_TEXTURE_2D,      //! ָ���Ƕ�άͼƬ
            0,                  //! ָ��Ϊ��һ�������������mipmap,��lod,����ľͲ��ü����ģ�Զ��ʹ�ý�С������
            GL_ALPHA,           //! �����ʹ�õĴ洢��ʽ
            _textureWidth,
            _textureHeight,
            0,                  //! �Ƿ�ı�
            GL_ALPHA,           //! ���ݵĸ�ʽ��bmp�У�windows,����ϵͳ�д洢��������bgr��ʽ
            GL_UNSIGNED_BYTE,   //! ������8bit����
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
                *   д��һ��,���¿�ʼ
                */
                _xStart =   0;
                /**
                *   y��ʼλ��Ҫ����
                */
                _yStart +=  tmax<int>(_fontPixelY,_fontSize);
            }
            FT_Load_Glyph((FT_Face) _face, FT_Get_Char_Index( (FT_Face)_face, ch ), FT_LOAD_DEFAULT );
            FT_Glyph glyph;
            FT_Glyph glyphSrc;
            FT_Get_Glyph( FT_Face(_face)->glyph, &glyphSrc );

            glyph   =   glyphSrc;

            /**
            *   ��������Ĵ�С�����Ƿ�ʹ�÷���ݻ���ģʽ
            *   ������Ƚ�С����˵����ʹ��ft_render_mode_mono
            *   ������Ƚϴ�������12���ϣ�����ʹ��ft_render_mode_normalģʽ
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
            *   ���û�����ݣ���д��ֱ�ӹ�ȥ
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
            *   ��һ����
            */
            vertex[index + 0].x       =   xStart;
            vertex[index + 0].y       =   yStart - h + offsetY;
            vertex[index + 0].z       =   zStart;
            vertex[index + 0].u       =   ch.x0/texWidth;
            vertex[index + 0].v       =   ch.y0/texHeight;
            vertex[index + 0].color   =   color;
            /**
            *   �ڶ�����
            */
            vertex[index + 1].x       =   xStart + w;
            vertex[index + 1].y       =   yStart - h + offsetY;
            vertex[index + 1].z       =   zStart;
            vertex[index + 1].u       =   ch.x1/texWidth;
            vertex[index + 1].v       =   ch.y0/texHeight;
            vertex[index + 1].color   =   color;
            /**
            *   ��������
            */
            vertex[index + 2].x       =   xStart + w;
            vertex[index + 2].y       =   yStart  + offsetY;
            vertex[index + 2].z       =   zStart;
            vertex[index + 2].u       =   ch.x1/texWidth;
            vertex[index + 2].v       =   ch.y1/texHeight;
            vertex[index + 2].color   =   color;
            /**
            *   ��һ����
            */
            vertex[index + 3].x       =   xStart;
            vertex[index + 3].y       =   yStart - h + offsetY;
            vertex[index + 3].z       =   zStart;
            vertex[index + 3].u       =   ch.x0/texWidth;
            vertex[index + 3].v       =   ch.y0/texHeight;
            vertex[index + 3].color   =   color;
            /**
            *   ��������
            */
            vertex[index + 4].x       =   xStart + w;
            vertex[index + 4].y       =   yStart  + offsetY;
            vertex[index + 4].z       =   zStart;
            vertex[index + 4].u       =   ch.x1/texWidth;
            vertex[index + 4].v       =   ch.y1/texHeight;
            vertex[index + 4].color   =   color;
            /**
            *   ���ĸ���
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
        *   ���������������.
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